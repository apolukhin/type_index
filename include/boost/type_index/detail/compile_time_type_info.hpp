//
// Copyright (c) Antony Polukhin, 2012-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP
#define BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP

/// \file compile_time_type_info.hpp
/// \brief Contains helper macros and implementation details of boost::typeindex::ctti_type_index.
/// Not intended for inclusion from user's code. 

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/bool.hpp>
#include <algorithm>
#include <string>

#if ! defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES ) && ! defined ( BOOST_NO_CXX11_CONSTEXPR ) && ! defined( BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION )
#include <boost/type_index/detail/constexpr_type_info.hpp>
#endif

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif


#if defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)
#define BOOST_TYPEINDEX_FUNCTION_NAME BOOST_TYPE_INDEX_FUNCTION_SIGNATURE
#elif defined(__FUNCSIG__)
#define BOOST_TYPEINDEX_FUNCTION_NAME __FUNCSIG__
#elif defined(__PRETTY_FUNCTION__) \
            || defined(__GNUC__) \
            || (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130)) \
            || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) \
            || (defined(__ICC) && (__ICC >= 600)) \
            || defined(__ghs__) \
            || defined(__DMC__)
#define BOOST_TYPEINDEX_FUNCTION_NAME __PRETTY_FUNCTION__
#else
    BOOST_STATIC_ASSERT_MSG(
        sizeof(T) && false,
        "TypeIndex library could not detect your compiler. "
        "Please make the BOOST_TYPE_INDEX_FUNCTION_SIGNATURE macro use "
        "correct compiler macro for getting the whole function name. "
        "Define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING to correct value after that."
    );
#endif

/// @cond
#define BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(begin_skip, end_skip, runtime_skip, runtime_skip_until)   \
    namespace boost { namespace typeindex { namespace detail {                                                  \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_begin  = begin_skip;                               \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_end    = end_skip;                                 \
        BOOST_STATIC_CONSTEXPR bool ctti_skip_more_at_runtime       = runtime_skip;                             \
        BOOST_STATIC_CONSTEXPR char ctti_skip_until_runtime[]       = runtime_skip_until;                       \
    }}} /* namespace boost::typeindex::detail */                                                                \
    /**/  
/// @endcond

#define BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS_TYPE(additional_offset)                                   \
    namespace boost { namespace typeindex { namespace detail {                                                  \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_begin_type  = additional_offset;                   \
    }}} /* namespace boost::typeindex::detail */

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)
    /* Nothing to document. All the macro docs are moved to <boost/type_index.hpp> */
#elif defined(BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING)

#   include <boost/preprocessor/facilities/expand.hpp>
    BOOST_PP_EXPAND( BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING )

#elif defined(_MSC_VER)

#if defined (BOOST_NO_CXX11_NOEXCEPT)
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1, sizeof(">::n(void)") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(40, 10, false, "")
#else
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1, sizeof(">::n(void) noexcept") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(40, 19, false, "")
#endif
#elif defined(__clang__) && defined(__APPLE__)
    // Someone made __clang_major__ equal to LLVM version rather than compiler version
    // on APPLE platform.
    //
    // Using less efficient solution because there is no good way to detect real version of Clang.
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof("]") - 1, true, "???????????>::n() [T = int"
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 1, true, "T = ")
#elif defined(__clang__) && (__clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ == 0))
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof(">::n()") - 1
    // note: checked on 3.0
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 6, false, "")
#elif defined(__clang__) && __clang_major__ == 3 && __clang_minor__ > 0
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof("]") - 1, true, "int>::n() [T = int"
    // note: checked on 3.1, 3.4
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 1, true, "T = ")
#elif defined(__GNUC__)
    // sizeof("static const char* boost::detail::ctti<T>::n() [with T = ") - 1, sizeof("]") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(57, 1, false, "")
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS_TYPE(40)
#else
    // Deafult code for other platforms... Just skip nothing!
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(0, 0, false, "")
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS_TYPE(0)
#endif

#undef BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS

namespace boost { namespace typeindex { namespace detail { 
    template <bool Condition>
    inline void assert_compile_time_legths() BOOST_NOEXCEPT {
        BOOST_STATIC_ASSERT_MSG(
            Condition,
            "TypeIndex library is misconfigured for your compiler. "
            "Please define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING to correct values. See section "
            "'RTTI emulation limitations' of the documentation for more information."
        );
    }
    
    template <std::size_t ArrayLength>
    inline const char* skip_begining_runtime(const char* begin, boost::mpl::false_) BOOST_NOEXCEPT {
        return begin;
    }

    template <std::size_t ArrayLength>
    inline const char* skip_begining_runtime(const char* begin, boost::mpl::true_) BOOST_NOEXCEPT {
        const char* const it = std::search(
            begin, begin + ArrayLength,
            ctti_skip_until_runtime, ctti_skip_until_runtime + sizeof(ctti_skip_until_runtime) - 1
        );
        return (it == begin + ArrayLength ? begin : it + sizeof(ctti_skip_until_runtime) - 1);
    }

    template <std::size_t ArrayLength>
    inline const char* skip_begining(const char* begin) BOOST_NOEXCEPT {
        assert_compile_time_legths<(ArrayLength > ctti_skip_size_at_begin + ctti_skip_size_at_end)>();
        return skip_begining_runtime<ArrayLength - ctti_skip_size_at_begin>(
            begin + ctti_skip_size_at_begin, 
            boost::mpl::bool_<ctti_skip_more_at_runtime>()
        );
    }


#if ! defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES ) && ! defined ( BOOST_NO_CXX11_CONSTEXPR ) && ! defined( BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION )

template<std::size_t ... Idx>
struct index_seq
{
};

template<typename T, std::size_t Append>
struct append;

template<std::size_t ...indexes, std::size_t Append>
struct append<index_seq<indexes...>, Append>
{
    using type = index_seq<indexes ..., Append>;
};

template<typename T, std::size_t Append>
using append_t = typename append<T, Append>::type;



template<std::size_t Size, std::size_t Counter = 0, typename T = index_seq<>>
struct make_index_seq
{
    typedef typename make_index_seq<Size, Counter+1, append_t<T, Counter>>::type type;
};

template<std::size_t Size, typename T>
struct make_index_seq<Size, Size, T>
{
    typedef T type;
};

template<std::size_t Size, std::size_t Start = 0>
using make_index_seq_t = typename make_index_seq<Size, Start>::type;


#endif

}

}} // namespace boost::typeindex::detail

namespace boost { namespace detail {

/// Noncopyable type_info that does not require RTTI.
/// CTTI == Compile Time Type Info.
/// This name must be as short as possible, to avoid code bloat
template <class T>
struct ctti {

    /// Returns raw name. Must be as short, as possible, to avoid code bloat
    static const char* n() BOOST_NOEXCEPT {
        return boost::typeindex::detail::skip_begining< sizeof(BOOST_TYPEINDEX_FUNCTION_NAME) >(BOOST_TYPEINDEX_FUNCTION_NAME);
    }
#if ! defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES ) && ! defined ( BOOST_NO_CXX11_CONSTEXPR ) && ! defined( BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION )
    //helper functions

    template<std::size_t Index>
    constexpr static char s() //step
    {
        constexpr std::size_t offset =
                  (Index >= 10u      ? 1u : 0u)
                + (Index >= 100u     ? 1u : 0u)
                + (Index >= 1000u    ? 1u : 0u)
                + (Index >= 10000u   ? 1u : 0u)
                + (Index >= 100000u  ? 1u : 0u)
                + (Index >= 1000000u ? 1u : 0u);
        return BOOST_TYPEINDEX_FUNCTION_NAME[Index + offset];
    }
    template<std::size_t ...Indexes>
    constexpr static auto impl(::boost::typeindex::detail::index_seq<Indexes...> )
    {
        return ::boost::typeindex::detail::constexpr_typename<s<Indexes>()...>();
    }
    template<std::size_t Dummy = 0>
    constexpr static auto m()
    {
        using idx_seq = boost::typeindex::detail::make_index_seq_t<
                sizeof(BOOST_TYPEINDEX_FUNCTION_NAME)
                - boost::typeindex::detail::ctti_skip_size_at_end - 1, //minus one, because it's the size
                boost::typeindex::detail::ctti_skip_size_at_begin
                + boost::typeindex::detail::ctti_skip_size_at_begin_type>;
        return impl(idx_seq());
    }
#endif
};

}} // namespace boost::detail

#endif // BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP

