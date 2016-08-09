//
// Copyright (c) Chris Glover, 2016.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_RUNTIME_CAST_REGISTER_RUNTIME_CLASS_HPP
#define BOOST_TYPE_INDEX_RUNTIME_CAST_REGISTER_RUNTIME_CLASS_HPP

/// \file register_runtime_class.hpp
/// \brief Contains the macro BOOST_TYPE_INDEX_REGISTER_RUNTIME_CLASS
///
/// BOOST_TYPE_INDEX_REGISTER_RUNTIME_CLASS is used to provide 
/// boost::typeindex::runtime_cast with the information necessary to perform
/// the dynamic_cast emulation it was designed to do.

#include <boost/type_index.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost { namespace typeindex {

namespace detail {

template<typename T>
inline type_index runtime_class_construct_type_id(T const*) {
    return type_id<T>();
}

} // namespace detail

#define BOOST_TYPE_INDEX_CHECK_BASE_(r, data, Base) \
    if(void const* ret_val = this->Base::boost_type_index_find_instance_(idx)) return ret_val;

#define BOOST_TYPE_INDEX_CHECK_BASES(base_list) \
    BOOST_PP_SEQ_FOR_EACH(BOOST_TYPE_INDEX_CHECK_BASE_, _, base_list)

#define BOOST_TYPE_INDEX_REGISTER_RUNTIME_CLASS                                                                           \
    virtual void const* boost_type_index_find_instance_(boost::typeindex::type_index const& idx) const BOOST_NOEXCEPT {   \
        if(idx == boost::typeindex::detail::runtime_class_construct_type_id(this))                                        \
            return this;                                                                                                  \
        return NULL;                                                                                                      \
    }                                                                   

#define BOOST_TYPE_INDEX_REGISTER_RUNTIME_CLASS_BASES(base_list)                                                          \
    virtual void const* boost_type_index_find_instance_(boost::typeindex::type_index const& idx) const BOOST_NOEXCEPT {   \
        if(idx == boost::typeindex::detail::runtime_class_construct_type_id(this))                                        \
            return this;                                                                                                  \
         BOOST_TYPE_INDEX_CHECK_BASES(base_list)                                                                          \
         return NULL;                                                                                                     \
    }  
}} // namespace boost::typeindex

#endif // BOOST_TYPE_INDEX_RUNTIME_CAST_HPP