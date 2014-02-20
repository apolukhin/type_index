//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_CTTI_REGISTER_CLASS_HPP
#define BOOST_TYPE_INDEX_CTTI_REGISTER_CLASS_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file ctti_register_class.hpp
/// \brief Contains BOOST_TYPE_INDEX_REGISTER_CTTI_CLASS macro.

#include <boost/type_index/ctti_type_index.hpp>

namespace boost { namespace typeind { namespace detail {

template <class T>
inline const ctti_data& ctti_construct_typeid_ref(const T*) BOOST_NOEXCEPT {
    return ctti_construct<T>();
}

}}} // namespace boost::typeind::detail


#define BOOST_TYPE_INDEX_REGISTER_CTTI_CLASS                                                    \
    virtual const boost::typeind::detail::ctti_data& type_id_runtime() const BOOST_NOEXCEPT {   \
        return boost::typeind::detail::ctti_construct_typeid_ref(this);                         \
    }                                                                                           \
/**/

#endif // BOOST_TYPE_INDEX_CTTI_REGISTER_CLASS_HPP

