//
// Copyright Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE testing_both_test_module
#include <boost/test/unit_test.hpp>
#include <boost/type_index/type_index_minimal.hpp>

#ifndef BOOST_NO_RTTI
#error "This test must be run with disabled RTTI"
#endif

namespace my_namespace1 {
    class my_class{};
}


namespace my_namespace2 {
    class my_class{};
}

#include "type_index_tests.ipp"

