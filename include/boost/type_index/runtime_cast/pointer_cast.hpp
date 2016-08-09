//
// Copyright (c) Chris Glover, 2016.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_RUNTIME_CAST_POINTER_CAST_HPP
#define BOOST_TYPE_INDEX_RUNTIME_CAST_POINTER_CAST_HPP

/// \file pointer_class.hpp
/// \brief Contains the overload of boost::typeindex::runtime_cast for
/// pointer types.
///
/// boost::typeindex::runtime_cast can be used to emulate dynamic_cast
/// functionality on platorms that don't provide it or should the user
/// desire opt in functionality instead of enabling it system wide.

#include <boost/type_index.hpp>
#include <boost/type_index/runtime_cast/detail/runtime_cast_impl.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost { namespace typeindex {

template<typename T, typename U>
T runtime_cast(U* u) BOOST_NOEXCEPT {
    typedef typename boost::remove_pointer<T>::type impl_type;
    return detail::runtime_cast_impl<impl_type>(u, boost::is_base_and_derived<T, U>());
} 

template<typename T, typename U>
T runtime_cast(U const* u) BOOST_NOEXCEPT {
    typedef typename boost::remove_pointer<T>::type impl_type;
    return detail::runtime_cast_impl<impl_type>(u, boost::is_base_and_derived<T, U>());
}

template<typename T, typename U>
T* runtime_pointer_cast(U* u) BOOST_NOEXCEPT {
    return detail::runtime_cast_impl<T>(u, boost::is_base_and_derived<T, U>());
} 

template<typename T, typename U>
T const* runtime_pointer_cast(U const* u) BOOST_NOEXCEPT {
    return detail::runtime_cast_impl<T>(u, boost::is_base_and_derived<T, U>());
}

}} // namespace boost::typeindex

#endif // BOOST_TYPE_INDEX_RUNTIME_CAST_POINTER_CAST_HPP