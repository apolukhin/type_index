//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_STL_REGISTER_CLASS_HPP
#define BOOST_TYPE_INDEX_STL_REGISTER_CLASS_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file stl_register_class.hpp
/// \brief Contains BOOST_TYPE_INDEX_REGISTER_STL_CLASS macro.

#include <boost/type_index/stl_type_index.hpp>

namespace boost { namespace typeindex { namespace detail {

template <class T>
inline const stl_type_index::type_info_t& stl_construct_typeid_ref(const T*) BOOST_NOEXCEPT {
    return typeid(T);
}

}}} // namespace boost::typeindex::detail


/// \def BOOST_TYPE_INDEX_REGISTER_STL_CLASS
/// BOOST_TYPE_INDEX_REGISTER_STL_CLASS is used by BOOST_TYPE_INDEX_REGISTER_CLASS when RTTI is off
/// and `typeid()` does work.
///
/// BOOST_TYPE_INDEX_REGISTER_STL_CLASS macro expands to declaration and implementation of
/// `virtual const std::type_info& type_id_runtime() const` method.
#define BOOST_TYPE_INDEX_REGISTER_STL_CLASS                                                             \
    virtual const boost::typeindex::stl_type_index::type_info_t& type_id_runtime() const BOOST_NOEXCEPT { \
        return boost::typeindex::detail::stl_construct_typeid_ref(this);                                  \
    }                                                                                                   \
/**/

#endif // BOOST_TYPE_INDEX_STL_REGISTER_CLASS_HPP

