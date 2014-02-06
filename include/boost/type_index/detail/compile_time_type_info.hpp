//
// Copyright (c) Antony Polukhin, 2012-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP
#define BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif


/// \file compile_time_type_info.hpp
/// \brief Contains implementation of boost::ctti class.
///

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace typeind { namespace detail { 

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def BOOST_TYPE_INDEX_FUNCTION_SIGNATURE
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE is used by boost::template_info class to
/// deduce the name of a template parameter. If your compiler is not recognized 
/// by the TypeIndex library and you wish to work with boost::template_info, you may 
/// define this macro by yourself.
///
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE must be defined to a compiler specific macro, 
/// that outputs the WHOLE function signature, including template parameters.
///
/// If your compiler is not recognised and BOOST_TYPE_INDEX_FUNCTION_SIGNATURE is not defined,
/// then a compile-time error will arise at any attempt to use boost::template_info or 
/// boost::template_index classes.
#define BOOST_TYPE_INDEX_FUNCTION_SIGNATURE BOOST_CURRENT_FUNCTION

/// \def BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP
///
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE, BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP 
/// and BOOST_TYPE_INDEX_CTTI_END_SKIP macroses are used for adding a 
/// support for compilers, that by default are not recognized by TypeIndex library.
///
/// See Compiler support for more info
#define BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP 0

/// \def BOOST_TYPE_INDEX_CTTI_END_SKIP
///
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE, BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP 
/// and BOOST_TYPE_INDEX_CTTI_END_SKIP macroses are used for adding a 
/// support for compilers, that by default are not recognized by TypeIndex library.
///
/// See Compiler support for more info
#define BOOST_TYPE_INDEX_CTTI_END_SKIP 0

#elif defined(BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP) && defined(BOOST_TYPE_INDEX_CTTI_END_SKIP)
    // skip user specified bytes count
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP);
    // skip user specified bytes count
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = BOOST_TYPE_INDEX_CTTI_END_SKIP);
#elif defined _MSC_VER
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 40);

    // sizeof(">::n(void)") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 10);

#elif defined __clang__
    // sizeof("static const char *boost::detail::ctti<") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 39);

    // == sizeof(">::n()") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 6);
#elif defined __GNUC__
    // sizeof("static const char* boost::detail::ctti<T>::n() [with T = ") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 57);

    // == sizeof("]") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 1);

#else
    // TODO: Code for other platforms
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 0);   // skip nothing
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 0);     // skip nothing
#endif

}}} // namespace boost::typeind::detail

namespace boost { namespace detail {

/// Noncopyable type_info that does not require RTTI.
/// CTTI == Compile Time Type Info.
/// This name must be as short as posible, to avoid code bloat
template <class T>
struct ctti {
    typedef T          template_type;
    typedef ctti       this_type;

    /// Returns raw name. Must be as short, as possible, to avoid code bloat
    static const char* n() BOOST_NOEXCEPT {
        #if defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)
            return BOOST_TYPE_INDEX_FUNCTION_SIGNATURE + boost::typeind::detail::ctti_skip_size_at_begin;
        #elif defined(__FUNCSIG__)
            return __FUNCSIG__ + boost::typeind::detail::ctti_skip_size_at_begin;
        #elif defined(__PRETTY_FUNCTION__) \
                    || defined(__GNUC__) \
                    || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) \
                    || (defined(__ICC) && (__ICC >= 600)) \
                    || defined(__ghs__) \
                    || defined(__DMC__)

            return __PRETTY_FUNCTION__ + boost::typeind::detail::ctti_skip_size_at_begin;
        #else
                BOOST_STATIC_ASSERT_MSG(
                    sizeof(T) && false,
                    "TypeIndex library could not detect your compiler. "
                    "Please make the BOOST_TYPE_INDEX_FUNCTION_SIGNATURE macro use "
                    "correct compiler macro for getting the whole function name. "
                    "Do not forget to also define BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP and "
                    "BOOST_TYPE_INDEX_CTTI_END_SKIP."
                );
        #endif
    }

    /// Returns raw name
    static const char* name() BOOST_NOEXCEPT {
        return this_type::n();
    }
};

}} // namespace boost::detail

#endif // BOOST_TYPE_INDEX_DETAIL_COMPILE_TIME_TYPE_INFO_HPP

