//
// Copyright Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE testing_crossmodule_module
#include <boost/test/unit_test.hpp>

#include <boost/type_index/type_index_minimal.hpp>
#include "test_lib.hpp"

namespace user_defined_namespace {
    class user_defined{};
}

BOOST_AUTO_TEST_CASE(comparing_types_between_modules)
{
    boost::type_index t_const_int = boost::type_id_with_cvr<const int>();
    boost::type_index t_int = boost::type_id<int>();

    BOOST_CHECK_EQUAL(t_int, test_lib::get_integer());
    BOOST_CHECK_EQUAL(t_const_int, test_lib::get_const_integer());
    BOOST_CHECK_NE(t_const_int, test_lib::get_integer());
    BOOST_CHECK_NE(t_int, test_lib::get_const_integer());


    boost::type_index t_const_userdef = boost::type_id_with_cvr<const user_defined_namespace::user_defined>();
    boost::type_index t_userdef = boost::type_id<user_defined_namespace::user_defined>();

    BOOST_CHECK_EQUAL(t_userdef, test_lib::get_user_defined_class());
    BOOST_CHECK_EQUAL(t_const_userdef, test_lib::get_const_user_defined_class());
    BOOST_CHECK_NE(t_const_userdef, test_lib::get_user_defined_class());
    BOOST_CHECK_NE(t_userdef, test_lib::get_const_user_defined_class());


    BOOST_CHECK_NE(t_userdef, test_lib::get_integer());
    BOOST_CHECK_NE(t_const_userdef, test_lib::get_integer());
    BOOST_CHECK_NE(t_int, test_lib::get_user_defined_class());
    BOOST_CHECK_NE(t_const_int, test_lib::get_const_user_defined_class());
}

