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
/// By inclusion of this file both classes (`type_index` (if RTTI is on) and `template_index`) will be available.
/// Consider including `<boost/type_index/type_index_minimal.hpp>` if you do not whant to include `template_index` class
/// while RTTI is available (this is recommended).

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/type_index/type_index_minimal.hpp>
#include <boost/type_index/template_index_impl.hpp>

#endif // BOOST_TYPE_INDEX_HPP

