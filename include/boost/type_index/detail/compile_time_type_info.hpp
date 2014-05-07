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

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/bool.hpp>
#include <algorithm>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

/// \def BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(begin_skip, end_skip, runtime_skip, runtime_skip_until)
/// This is a helper macro for making correct pretty_names() with RTTI off.
/// 
/// \b Example:
/// 
/// Imagine the situation when 
/// \code boost::typeindex::ctti_type_index::type_id<int>().pretty_name() \endcode 
/// returns the following string:
/// \code "static const char *boost::detail::ctti<int>::n() [T = int]" \endcode
/// and \code boost::typeindex::ctti_type_index::type_id<short>().pretty_name() \endcode returns the following:
/// \code "static const char *boost::detail::ctti<short>::n() [T = short]" \endcode
///
/// As we may see first 39 characters are "static const char *boost::detail::ctti<" and they do not depend on 
/// the type T. After first 39 characters we have a human readable type name which is duplicated at the end 
/// of a string. String always ends on ']', which consumes 1 character.
///
/// Now if we define `BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING` to 
/// `BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING(39, 1, false, "")` we'll be getting \code "int>::n() [T = int" \endcode 
/// for `boost::typeindex::ctti_type_index::type_id<int>().pretty_name()` and \code "short>::n() [T = short" \endcode 
/// for `boost::typeindex::ctti_type_index::type_id<short>().pretty_name()`.
///
/// Now we need to take additional care of the characters that go before the last mention of our type. We'll 
/// do that by telling the macro that we need to cut off everything that goes before the "T = " including the "T = "
/// itself:
///
/// \code BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING(39, 1, true, "T = ") \endcode 
///
/// In case of GCC or Clang command line we need to add the following line while compiling al lthe sources:
///
/// \code
/// -DBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING='BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING(39, 1, true, "T = ")'
/// \endcode
/// \param begin_skip How many characters must be skipped at the beginning of the type holding string. 
/// Must be a compile time constant.
/// \param end_skip How many characters must be skipped at the end of the type holding string. 
/// Must be a compile time constant.
/// \param runtime_skip Do we need additional checks at runtime to cut off the more characters.
/// Must be `true` or `false`.l
/// \param runtime_skip_until Skip all the characters before the following string (including the string itself).
/// Must be a compile time array of characters.
#define BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(begin_skip, end_skip, runtime_skip, runtime_skip_until)   \
    namespace boost { namespace typeindex { namespace detail {                                                  \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_begin  = begin_skip;                               \
        BOOST_STATIC_CONSTEXPR std::size_t ctti_skip_size_at_end    = end_skip;                                 \
        BOOST_STATIC_CONSTEXPR bool ctti_skip_more_at_runtime       = runtime_skip;                             \
        BOOST_STATIC_CONSTEXPR char ctti_skip_until_runtime[]       = runtime_skip_until;                       \
    }}} /* namespace boost::typeindex::detail */                                                                \
    /**/  


#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def BOOST_TYPE_INDEX_FUNCTION_SIGNATURE
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE is used by boost::typeindex::ctti_type_index class to
/// deduce the name of a type. If your compiler is not recognized 
/// by the TypeIndex library and you wish to work with boost::typeindex::ctti_type_index, you may 
/// define this macro by yourself.
///
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE must be defined to a compiler specific macro
/// that outputs the \b whole function signature \b including \b template \b parameters.
///
/// If your compiler is not recognised and BOOST_TYPE_INDEX_FUNCTION_SIGNATURE is not defined,
/// then a compile-time error will arise at any attempt to use boost::typeindex::ctti_type_index classes.
/// 
/// See BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS and BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING 
/// for an information of how to tune the implementation to make a nice retty_name() output.
#define BOOST_TYPE_INDEX_FUNCTION_SIGNATURE BOOST_CURRENT_FUNCTION

/// \def BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING
///
/// BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING macro may be defined to 
/// BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(begin_skip, end_skip, runtime_skip, runtime_skip_until) with parameters for adding a 
/// support for compilers, that by default are not recognized by TypeIndex library.
///
/// See 'RTTI emulation limitations' for more info
#define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(0, 0, false, "");

#elif defined(BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING)
    BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING
#elif defined(_MSC_VER)
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1, sizeof(">::n(void)") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(40, 10, false, "");
#elif defined(__clang__) && (__clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ == 0))
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof(">::n()") - 1
    // note: checked on 3.0
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 6, false, "");
#elif defined(__clang__) && __clang_major__ == 3 && __clang_minor__ > 0
    // sizeof("static const char *boost::detail::ctti<") - 1, sizeof("]") - 1, true, "int>::n() [T = int"
    // note: checked on 3.1, 3.4
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(39, 1, true, "T = ");
#elif defined(__GNUC__)
    // sizeof("static const char* boost::detail::ctti<T>::n() [with T = ") - 1, sizeof("]") - 1
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(57, 1, false, "");
#else
    // Deafult code for other platforms... Just skip nothing!
    BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS(0, 0, false, "");
#endif

#undef BOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS

namespace boost { namespace typeindex { namespace detail { 
    template <bool Condition>
    inline void assert_compile_time_legths() BOOST_NOEXCEPT {
        BOOST_STATIC_ASSERT_MSG(
            Condition,
            "TypeIndex library is misconfigured for your compiler. "
            "Please define BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP and "
            "BOOST_TYPE_INDEX_CTTI_END_SKIP to correct values. See section "
            "'RTTI emulation limitations' of the documentation."
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
}}} // namespace boost::typeindex::detail

namespace boost { namespace detail {

/// Noncopyable type_info that does not require RTTI.
/// CTTI == Compile Time Type Info.
/// This name must be as short as posible, to avoid code bloat
template <class T>
struct ctti {

    /// Returns raw name. Must be as short, as possible, to avoid code bloat
    static const char* n() BOOST_NOEXCEPT {
    #if defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)
        return boost::typeindex::detail::skip_begining< sizeof(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE >(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE);
    #elif defined(__FUNCSIG__)
        return boost::typeindex::detail::skip_begining< sizeof(__FUNCSIG__) >(__FUNCSIG__);
    #elif defined(__PRETTY_FUNCTION__) \
                || defined(__GNUC__) \
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
};

}} // namespace boost::detail

#endif // BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP

