//
// Copyright (c) Chris Glover, 2016.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_RUNTIME_REFERENCE_CAST_HPP
#define BOOST_TYPE_INDEX_RUNTIME_REFERENCE_CAST_HPP

/// \file reference_cast.hpp
/// \brief Contains the overload of boost::typeindex::runtime_cast for
/// reference types.

#include <boost/type_index/runtime_cast/detail/runtime_cast_impl.hpp>
#include <boost/throw_exception.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <typeinfo>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost { namespace typeindex {

template<typename T, typename U>
T& runtime_cast(U& u) {
    typedef typename boost::remove_reference<T>::type impl_type;
    impl_type* value = detail::runtime_cast_impl<impl_type>(&u, boost::is_base_and_derived<T, U>());
    if(!value)
        boost::throw_exception(std::bad_cast());
    return *value;
} 

template<typename T, typename U>
T const& runtime_cast(U const& u) {
    typedef typename boost::remove_reference<T>::type impl_type;
    impl_type* value = detail::runtime_cast_impl<impl_type>(&u, boost::is_base_and_derived<T, U>());
    if(!value)
        boost::throw_exception(std::bad_cast());
    return *value;
}

}} // namespace boost::typeindex

#endif // BOOST_TYPE_INDEX_RUNTIME_REFERENCE_CAST_HPP