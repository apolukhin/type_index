//
// Copyright (c) Antony Polukhin, 2012-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_HPP

/// \file boost/type_index.hpp
/// \brief Includes all the headers of the Boost.TypeIndex library.
///
/// By inclusion of this file all classes (boost::type_info + boost::type_index if RTTI is on 
/// and boost::template_info + boost::template_index) will be available.
///
/// Consider including <boost/type_index/type_index.hpp> if you do not want to include 
/// boost::template_info and boost::template_index class while RTTI is available.
///
/// Consider including <boost/type_index/type_info.hpp> if you need only boost::type_info class 
/// and boost::type_id* functions.

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>

#if defined(BOOST_TYPE_INDEX_USER_TYPEINFO) && defined(BOOST_TYPE_INDEX_USER_TYPEINFO_NAME) 
#   include BOOST_TYPE_INDEX_USER_TYPEINFO
#elif (!defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(BOOST_MSVC)
#   include <boost/type_index/stl_type_index.hpp>
#else 
#   include <boost/type_index/ctti_type_index.hpp>
#endif

namespace boost { namespace typeind {

#if defined(BOOST_TYPE_INDEX_USER_TYPEINFO) && defined(BOOST_TYPE_INDEX_USER_TYPEINFO_NAME) 
    typedef BOOST_TYPE_INDEX_USER_TYPEINFO_NAME type_index;
#elif (!defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(BOOST_MSVC)
    typedef boost::typeind::stl_type_index type_index;
#else 
    typedef boost::typeind::ctti_type_index type_index;
#endif

typedef type_index::type_info_t type_info;

// TODO:
// inline bool is_equal(const type_info&, const type_info&) BOOST_NOEXCEPT;
// inline bool is_before(const type_info&, const type_info&) BOOST_NOEXCEPT;


/// Function to get boost::type_index for a type T. 
/// Strips const, volatile and & modifiers from T.
template <class T>
inline type_index type_id() BOOST_NOEXCEPT {
    return type_index::construct<T>();
}

/// Function for constructing boost::type_index instance for type T.
/// Does not strip const, volatile, & and && modifiers from T.
/// If T has no const, volatile, & and && modifiers, then returns exactly 
/// the same result as in case of calling `type_id<T>()`.
template <class T>
inline type_index type_id_with_cvr() BOOST_NOEXCEPT {
    return type_index::construct_with_cvr<T>();
}

/// Function that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
template <class T>
inline type_index type_id_runtime(const T& runtime_val) BOOST_NOEXCEPT {
    return type_index::construct_runtime(runtime_val);
}

/// Function that works exactly like C++ typeid(rtti_val) call, but returns boost::type_info.
template <class T>
inline type_index type_id_runtime(const T* runtime_val) {
    return type_index::construct_runtime(runtime_val);
}

}} // namespace boost::typeind


#endif // BOOST_TYPE_INDEX_HPP

