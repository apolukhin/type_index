//
// Copyright (c) Antony Polukhin, 2012-2013.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TYPE_INFO_HPP
#define BOOST_TYPE_INDEX_TYPE_INFO_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file type_index_minimal.hpp
/// \brief This is the header that required for ussage of boost::type_index with/without RTTI.
///
/// It includes only the minamally required headers and does the 'typedef template_index type_index;'
/// when RTTI is disabled.

#include <boost/config.hpp>

#if !defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NORTTI_COMPATIBILITY)
#   include <boost/type_index/type_info_impl.hpp>
#else
#   include <boost/type_index/template_info.hpp>
#   include <boost/static_assert.hpp>

namespace boost {

typedef template_index type_index;

template <class T>
inline const type_index& type_id() BOOST_NOEXCEPT {
    return template_index::construct<T>();
}

template <class T>
inline const type_index& type_id_with_cvr() BOOST_NOEXCEPT {
    return template_index::construct_with_cvr<T>();
}

template <class T>
inline const type_index& type_id_rtti_only(T& rtti_val) BOOST_NOEXCEPT {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, "boost::type_id_rtti_only(T&) requires RTTI");
    return type_index();
}

template <class T>
inline const type_index& type_id_rtti_only(T* rtti_val) {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, "boost::type_id_rtti_only(T*) requires RTTI");
    return type_index();
}

} // namespace boost

#endif // BOOST_NO_RTTI

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)
/// \def BOOST_TYPE_INDEX_FORCE_NORTTI_COMPATIBILITY
/// Define the BOOST_TYPE_INDEX_FORCE_NORTTI_COMPATIBILITY macro if you are mixing objects
/// compiled with different RTTI flags. This will force the usage of boost::template_index 
/// class instead of boost::type_index.
#define BOOST_TYPE_INDEX_FORCE_NORTTI_COMPATIBILITY
#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED

#endif // BOOST_TYPE_INDEX_TYPE_INFO_HPP

