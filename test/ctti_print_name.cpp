//
// Copyright Antony Polukhin, 2012-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE type_index_ctti_test_module
#include <boost/test/unit_test.hpp>


// This cpp file:
// * tests BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING macro
// * outputs full ctti name so that TypeIndex library could be adjust to new compiler without requesting regression tester's help
#define BOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING (0,0,false,"")
#include <boost/type_index.hpp>

namespace user_defined_namespace {
    class user_defined_class {};
}

BOOST_AUTO_TEST_CASE(ctti_print_full_string_helper_test)
{
    using namespace boost::typeindex;
    BOOST_TEST_CHECKPOINT("int: " << ctti_type_index::type_id<int>());
    BOOST_TEST_CHECKPOINT("double: " << ctti_type_index::type_id<double>());
    BOOST_TEST_CHECKPOINT("user_defined_namespace::user_defined_class: " << ctti_type_index::type_id<user_defined_namespace::user_defined_class>());

    BOOST_CHECK(true);
}

