//
// Copyright (c) Antony Polukhin, 2012-2015.
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

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

/// @cond
#define BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(begin_skip, end_skip, runtime_skip, runtime_skip_until, constexpr_begin_skip)     \
    namespace boost { namespace typeindex { namespace detail {                                                                          \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_begin  = begin_skip;                                                       \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_end    = end_skip;                                                         \
        BOOST_STATIC_CONSTEXPR bool ctti_skip_more_at_runtime       = runtime_skip;                                                     \
        BOOST_STATIC_CONSTEXPR char ctti_skip_until_runtime[]       = runtime_skip_until;                                               \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_constexpr_begin = constexpr_begin_skip;                                    \
    }}} /* namespace boost::typeindex::detail */                                                                                        \
    /**/
/// @endcond


#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)
    /* Nothing to document. All the macro docs are moved to <boost/type_index.hpp> */
#elif defined(BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING)

#   include <boost/preprocessor/facilities/expand.hpp>
    BOOST_PP_EXPAND( BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING )

#elif defined(_MSC_VER)

#if defined (BOOST_NO_CXX11_NOEXCEPT)
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1, sizeof(">::n(void)") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(40, 10, false, "", 0)
#else
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1, sizeof(">::n(void) noexcept") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(40, 19, false, "", 0)
#endif
#elif defined(__clang__) && defined(__APPLE__)
    // Someone made __clang_major__ equal to LLVM version rather than compiler version
    // on APPLE platform.
    //
    // Using less efficient solution because there is no good way to detect real version of Clang.
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof("]") - 1, true, "???????????>::n() [T = int"
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 1, true, "T = ", 0)
#elif defined(__clang__) && (__clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ == 0))
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof(">::n()") - 1
    // note: checked on 3.0
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 6, false, "", 0)
#elif defined(__clang__) && __clang_major__ == 3 && __clang_minor__ > 0
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof("]") - 1, true, "int>::n() [T = int"
    // note: checked on 3.1, 3.4
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 1, true, "T = ", 0)
#elif defined(__GNUC__)
    // sizeof("static const char* boost::detail::ctti<T>::n() [with T = ") - 1, sizeof("]") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(57, 1, false, "", 91)
#else
    // Deafult code for other platforms... Just skip nothing!
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(0, 0, false, "", 0)
#endif

#undef BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS

namespace boost { namespace typeindex { namespace detail { 
    template <bool Condition>
    BOOST_CXX14_CONSTEXPR inline void assert_compile_time_legths() BOOST_NOEXCEPT {
        BOOST_STATIC_ASSERT_MSG(
            Condition,
            "TypeIndex library is misconfigured for your compiler. "
            "Please define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING to correct values. See section "
            "'RTTI emulation limitations' of the documentation for more information."
        );
    }

    template <std::size_t ArrayLength>
    BOOST_CXX14_CONSTEXPR inline const char* skip_begining_runtime(const char* begin, boost::mpl::false_) BOOST_NOEXCEPT {
        return begin;
    }

    template<class ForwardIterator1, class ForwardIterator2>
    BOOST_CXX14_CONSTEXPR inline ForwardIterator1 constexpr_search(
        ForwardIterator1 first1,
        ForwardIterator1 last1,
        ForwardIterator2 first2,
        ForwardIterator2 last2) BOOST_NOEXCEPT
    {
        if (first2 == last2) {
            return first1;  // specified in C++11
        }

        while (first1 != last1) {
            ForwardIterator1 it1 = first1;
            ForwardIterator2 it2 = first2;

            while (*it1 == *it2) {
                ++it1;
                ++it2;
                if (it2 == last2) return first1;
                if (it1 == last1) return last1;
            }

            ++first1;
        }

        return last1;
    }

    BOOST_CXX14_CONSTEXPR inline int constexpr_strcmp(const char *v1, const char *v2) BOOST_NOEXCEPT {
        while (*v1 != '\0' && *v1 == *v2) {
            ++v1;
            ++v2;
        };

        return static_cast<int>(*v1) - *v2;
    }

    template <std::size_t ArrayLength>
    BOOST_CXX14_CONSTEXPR inline const char* skip_begining_runtime(const char* begin, boost::mpl::true_) BOOST_NOEXCEPT {
        const char* const it = constexpr_search(
            begin, begin + ArrayLength,
            ctti_skip_until_runtime, ctti_skip_until_runtime + sizeof(ctti_skip_until_runtime) - 1
        );
        return (it == begin + ArrayLength ? begin : it + sizeof(ctti_skip_until_runtime) - 1);
    }

    template <std::size_t ArrayLength>
    BOOST_CXX14_CONSTEXPR inline const char* skip_begining(const char* begin) BOOST_NOEXCEPT {
        assert_compile_time_legths<(ArrayLength > ctti_skip_size_at_begin + ctti_skip_size_at_end)>();
        return skip_begining_runtime<ArrayLength - ctti_skip_size_at_begin>(
            begin + ctti_skip_size_at_begin, 
            boost::mpl::bool_<ctti_skip_more_at_runtime>()
        );
    }
    
#if ! defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES ) && ! defined ( BOOST_NO_CXX11_CONSTEXPR )


    template <std::size_t Switch, class T0, class T1, class T2, class T3, class T4, class TDefault>
    struct switch_{
        typedef typename TDefault::type type;
    };
    
    template <class T0, class T1, class T2, class T3, class T4, class TDefault>
    struct switch_<0, T0, T1, T2, T3, T4, TDefault>{ typedef typename T0::type type; };
    
    template <class T0, class T1, class T2, class T3, class T4, class TDefault>
    struct switch_<1, T0, T1, T2, T3, T4, TDefault>{ typedef typename T1::type type; };
    
    template <class T0, class T1, class T2, class T3, class T4, class TDefault>
    struct switch_<2, T0, T1, T2, T3, T4, TDefault>{ typedef typename T2::type type; };
    
    template <class T0, class T1, class T2, class T3, class T4, class TDefault>
    struct switch_<3, T0, T1, T2, T3, T4, TDefault>{ typedef typename T3::type type; };
    
    template <class T0, class T1, class T2, class T3, class T4, class TDefault>
    struct switch_<4, T0, T1, T2, T3, T4, TDefault>{ typedef typename T4::type type; };


    template<std::size_t ... Idx>
    struct index_seq {};

    template<typename T, std::size_t... Append>
    struct append;

    template<std::size_t ...indexes, std::size_t... Append>
    struct append<index_seq<indexes...>, Append...> {
        using type = index_seq<indexes ..., Append...>;
    };

    template<typename T, std::size_t... Append>
    using append_t = typename append<T, Append...>::type;

    template<std::size_t Size, std::size_t Counter = 0, typename T = index_seq<>>
    struct make_index_seq {
        typedef typename switch_<
            Size - Counter - 1,
            make_index_seq<Size, Counter+1, append_t<T, Counter> >,
            make_index_seq<Size, Counter+2, append_t<T, Counter, Counter + 1> >,
            make_index_seq<Size, Counter+3, append_t<T, Counter, Counter + 1, Counter + 2> >,
            make_index_seq<Size, Counter+4, append_t<T, Counter, Counter + 1, Counter + 2, Counter + 3> >,
            make_index_seq<Size, Counter+5, append_t<T, Counter, Counter + 1, Counter + 2, Counter + 3, Counter + 4> >,
            make_index_seq<Size, Counter+6, append_t<T, Counter, Counter + 1, Counter + 2, Counter + 3, Counter + 4, Counter + 5> >
        >::type type;
    };

    template<std::size_t Size, typename T>
    struct make_index_seq<Size, Size, T> {
        typedef T type;
    };

    template<std::size_t Size, std::size_t Start = 0>
    using make_index_seq_t = typename make_index_seq<Size, Start>::type;


    
    template <char... C>
    struct cstring {
        static constexpr std::size_t size_ = sizeof...(C) + 1;
        static constexpr char data_[size_] = { C..., '\0' };
    };

    template <char... C>
    constexpr char cstring<C...>::data_[]; 
#endif

    
}}} // namespace boost::typeindex::detail

namespace boost { namespace detail {

/// Noncopyable type_info that does not require RTTI.
/// CTTI == Compile Time Type Info.
/// This name must be as short as possible, to avoid code bloat
template <class T>
struct ctti {
   
#if ! defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES ) && ! defined ( BOOST_NO_CXX11_CONSTEXPR )
    //helper functions
    template <std::size_t Index>
    constexpr static char s() { // step
        constexpr std::size_t offset =
                  (Index >= 10u      ? 1u : 0u)
                + (Index >= 100u     ? 1u : 0u)
                + (Index >= 1000u    ? 1u : 0u)
                + (Index >= 10000u   ? 1u : 0u)
                + (Index >= 100000u  ? 1u : 0u)
                + (Index >= 1000000u ? 1u : 0u)
        ;
        
    #if defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)
        return BOOST_TYPE_INDEX_FUNCTION_SIGNATURE[Index + offset];
    #elif defined(__FUNCSIG__)
        return __FUNCSIG__[Index + offset];
    #elif defined(__PRETTY_FUNCTION__) \
                || defined(__GNUC__) \
                || (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130)) \
                || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) \
                || (defined(__ICC) && (__ICC >= 600)) \
                || defined(__ghs__) \
                || defined(__DMC__)
        return __PRETTY_FUNCTION__[Index + offset];
    #else
        BOOST_STATIC_ASSERT_MSG(
            sizeof(T) && false,
            "TypeIndex library could not detect your compiler. "
            "Please make the BOOST_TYPE_INDEX_FUNCTION_SIGNATURE macro use "
            "correct compiler macro for getting the whole function name. "
            "Define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING to correct value after that."
        );
    #endif
    }
    
    template<std::size_t ...Indexes>
    constexpr static auto impl(::boost::typeindex::detail::index_seq<Indexes...> ) {
        return ::boost::typeindex::detail::cstring<s<Indexes>()...>();
    }
    
    template <std::size_t Dummy = 0>
    constexpr static auto n()
    {
        constexpr std::size_t size = sizeof(
        #if defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)
            BOOST_TYPE_INDEX_FUNCTION_SIGNATURE
        #elif defined(__FUNCSIG__)
            __FUNCSIG__
        #elif defined(__PRETTY_FUNCTION__) \
                    || defined(__GNUC__) \
                    || (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130)) \
                    || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) \
                    || (defined(__ICC) && (__ICC >= 600)) \
                    || defined(__ghs__) \
                    || defined(__DMC__)
            __PRETTY_FUNCTION__
        #else
            int
        #endif
        );
        
        using idx_seq = boost::typeindex::detail::make_index_seq_t<
                size - boost::typeindex::detail::ctti_skip_size_at_end,
                boost::typeindex::detail::ctti_skip_size_at_constexpr_begin
        >;
        return decltype( impl(idx_seq()) )::data_;
    }
#else
    /// Returns raw name. Must be as short, as possible, to avoid code bloat
    static const char* n() BOOST_NOEXCEPT {
    #if defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)
        return boost::typeindex::detail::skip_begining< sizeof(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE) >(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE);
    #elif defined(__FUNCSIG__)
        return boost::typeindex::detail::skip_begining< sizeof(__FUNCSIG__) >(__FUNCSIG__);
    #elif defined(__PRETTY_FUNCTION__) \
                || defined(__GNUC__) \
                || (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130)) \
                || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) \
                || (defined(__ICC) && (__ICC >= 600)) \
                || defined(__ghs__) \
                || defined(__DMC__)
        return boost::typeindex::detail::skip_begining< sizeof(__PRETTY_FUNCTION__) >(__PRETTY_FUNCTION__);
    #else
        BOOST_STATIC_ASSERT_MSG(
            sizeof(T) && false,
            "TypeIndex library could not detect your compiler. "
            "Please make the BOOST_TYPE_INDEX_FUNCTION_SIGNATURE macro use "
            "correct compiler macro for getting the whole function name. "
            "Define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING to correct value after that."
        );
    #endif
    }
#endif
};

}} // namespace boost::detail

#endif // BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP

