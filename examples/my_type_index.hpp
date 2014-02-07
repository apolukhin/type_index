// Copyright 2013-2014 Antony Polukhin

// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying file LICENSE_1_0.txt
// or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//[type_index_my_type_index
/*`
    The following example shows how to create an own type_info clas and how
    to use it all around the project.
*/

#ifndef MY_NAMESPACE_MY_TYPE_INDEX_HPP
#define MY_NAMESPACE_MY_TYPE_INDEX_HPP

// We'll use ctti_type_info as a base class for my_type_info
#include <boost/type_index/ctti_type_info.hpp>
namespace my_namespace {

struct my_type_index: public ctti_type_info {
    my_type_index(const ctti_type_info::type_info_t& data) BOOST_NOEXCEPT 
        : ctti_type_info(data)
    {}

    template <class T>
    static inline my_type_info construct_runtime(const T& value) BOOST_NOEXCEPT {
        return value.type_info();
    }
};

} // namespace my_namespace

#endif // MY_NAMESPACE_MY_TYPE_INDEX_HPP
//] [/type_index_my_type_index]

