//
// Copyright 2013-2026 Antony Polukhin.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_DETAIL_CONFIG_HPP
#define BOOST_TYPE_INDEX_DETAIL_CONFIG_HPP

#if defined __has_include
#  if __has_include (<version>)
#    include <version>
#  endif
#endif

#if __cplusplus < 202302L || __cpp_constexpr < 202211L || __cpp_lib_constexpr_typeinfo < 202105L
#define BOOST_TYPE_INDEX_IMPL_CTTI_DATA_VIA_CAST
#endif

#if defined(BOOST_TYPE_INDEX_IMPL_CTTI_DATA_VIA_CAST) && !defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)
#define BOOST_TYPE_INDEX_IMPL_TYPE_INFO_CONSTEXPR inline
#else
#define BOOST_TYPE_INDEX_IMPL_TYPE_INFO_CONSTEXPR constexpr
#endif

#if !defined(BOOST_TYPE_INDEX_INTERFACE_UNIT)
#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif
#endif

#ifdef BOOST_TYPE_INDEX_INTERFACE_UNIT
#   define BOOST_TYPE_INDEX_BEGIN_MODULE_EXPORT export {
#   define BOOST_TYPE_INDEX_END_MODULE_EXPORT }
#else
#   define BOOST_TYPE_INDEX_BEGIN_MODULE_EXPORT
#   define BOOST_TYPE_INDEX_END_MODULE_EXPORT
#endif

#if defined(BOOST_USE_MODULES) && !defined(BOOST_TYPE_INDEX_INTERFACE_UNIT)
import boost.type_index;
#endif

#endif // BOOST_TYPE_INDEX_DETAIL_CONFIG_HPP

