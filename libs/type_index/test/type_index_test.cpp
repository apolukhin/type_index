//
// Copyright Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE type_index_test_module
#include <boost/test/unit_test.hpp>

// Byapssing internal assertion for correct header inclusion
#define BOOST_TYPE_INDEX_TYPE_INFO_HPP

#include <boost/type_index/type_info_impl.hpp>

namespace my_namespace1 {
    class my_class{};
}


namespace my_namespace2 {
    class my_class{};
}

#include "type_index_tests.ipp"
