//
// Copyright (c) Antony Polukhin, 2012-2013.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TYPE_INDEX_MINIMAL_HPP
#define BOOST_TYPE_INDEX_TYPE_INDEX_MINIMAL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file type_index_minimal.hpp
/// \brief This is the header that required for ussage of type_index with/without RTTI.
///
/// It includes only the minamally required headers and does the `typedef template_index type_index;`
/// when RTTI is disabled.

#include <boost/config.hpp>

#ifndef BOOST_NO_RTTI
#   include <boost/type_index/type_index_impl.hpp>
#else
#   include <boost/type_index/template_index_impl.hpp>
#   include <boost/static_assert.hpp>

namespace boost {

typedef template_index type_index;

template <class T>
inline type_index type_id() BOOST_NOEXCEPT {
    return template_index::construct<T>();
}

template <class T>
inline type_index type_id_with_cvr() BOOST_NOEXCEPT {
    return template_index::construct_with_cvr<T>();
}

template <class T>
inline type_index type_id_rtti_only(T& rtti_val) BOOST_NOEXCEPT {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, "boost::type_id_rtti_only(T&) requires RTTI");
    return type_index();
}

template <class T>
inline type_index type_id_rtti_only(T* rtti_val) {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, "boost::type_id_rtti_only(T*) requires RTTI");
    return type_index();
}

} // namespace boost

#endif // BOOST_NO_RTTI


#endif // BOOST_TYPE_INDEX_TYPE_INDEX_MINIMAL_HPP

