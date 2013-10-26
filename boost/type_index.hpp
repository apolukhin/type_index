//
// Copyright (c) Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_HPP

/// \file type_index.hpp
/// \brief Includes all the headers of the Boost.TypeIndex library.
///
/// By inclusion of this file all classes (boost::type_info + boost::type_index if RTTI is on 
/// and boost::template_index) will be available.
///
/// Consider including <boost/type_index/type_index.hpp> if you do not whant to include 
/// boost::template_index class while RTTI is available.
///
/// Consider including <boost/type_index/type_info.hpp> if you do not whant to include 
/// boost::template_index and boost::type_index classes while RTTI is available.

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/type_index/type_info.hpp>
#include <boost/type_index/type_index.hpp>
#include <boost/type_index/template_info.hpp>

#endif // BOOST_TYPE_INDEX_HPP

