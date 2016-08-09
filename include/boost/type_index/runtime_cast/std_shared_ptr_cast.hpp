//
// Copyright (c) Chris Glover, 2016.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_RUNTIME_CAST_STD_SHARED_PTR_CAST_HPP
#define BOOST_TYPE_INDEX_RUNTIME_CAST_STD_SHARED_PTR_CAST_HPP

/// \file boost_shared_ptr_cast.hpp
/// \brief Contains the overload of boost::typeindex::runtime_pointer_cast for
/// std::shared_ptr types.

#include <boost/type_index/runtime_cast/detail/runtime_cast_impl.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <memory>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost { namespace typeindex {

template<typename T, typename U>
std::shared_ptr<T> runtime_pointer_cast(std::shared_ptr<U> const& u) {
    T* value = detail::runtime_cast_impl<T>(u.get(), boost::is_base_and_derived<T, U>());
    if(value)
        return std::shared_ptr<T>(u, value);
    return std::shared_ptr<T>();
} 

}} // namespace boost::typeindex

#endif // BOOST_TYPE_INDEX_RUNTIME_CAST_BOOST_SHARED_PTR_CAST_HPP