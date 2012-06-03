//
// Copyright Antony Polukhin, 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE type_index_test_module
#include <boost/test/unit_test.hpp>

#include <boost/type_index.hpp>

#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>

BOOST_AUTO_TEST_CASE(names_matches_type_id)
{
    using namespace boost;
    BOOST_CHECK_EQUAL(type_id<int>().name_demangled(), "int");
    BOOST_CHECK_EQUAL(type_id<double>().name_demangled(), "double");
    
    BOOST_CHECK_EQUAL(type_id<int>().name(),    type_id<int>().name());
    BOOST_CHECK_NE(type_id<int>().name(),       type_id<double>().name());
    BOOST_CHECK_NE(type_id<double>().name(),    type_id<int>().name());
    BOOST_CHECK_EQUAL(type_id<double>().name(), type_id<double>().name());
}

BOOST_AUTO_TEST_CASE(names_matches_template_id)
{
    using namespace boost;
    BOOST_CHECK_EQUAL(template_id<int>().name_demangled(), "int");
    BOOST_CHECK_EQUAL(template_id<double>().name_demangled(), "double");
    
    BOOST_CHECK_EQUAL(template_id<int>().name(),    template_id<int>().name());
    BOOST_CHECK_NE(template_id<int>().name(),       template_id<double>().name());
    BOOST_CHECK_NE(template_id<double>().name(),    template_id<int>().name());
    BOOST_CHECK_EQUAL(template_id<double>().name(), template_id<double>().name());
}

BOOST_AUTO_TEST_CASE(comparators_type_id)
{
    using namespace boost;
    type_index t_int = type_id<int>();
    type_index t_double = type_id<double>();

    BOOST_CHECK_EQUAL(t_int, t_int);
    BOOST_CHECK_LE(t_int, t_int);
    BOOST_CHECK_GE(t_int, t_int);
    BOOST_CHECK_NE(t_int, t_double);

    BOOST_CHECK_LE(t_double, t_double);
    BOOST_CHECK_GE(t_double, t_double);
    BOOST_CHECK_NE(t_double, t_int);

    BOOST_CHECK(t_double < t_int || t_int < t_double);
    BOOST_CHECK(t_double > t_int || t_int > t_double);
}

BOOST_AUTO_TEST_CASE(comparators_template_id)
{
    using namespace boost;
    template_index t_int = template_id<int>();
    template_index t_double = template_id<double>();

    BOOST_CHECK_EQUAL(t_int, t_int);
    BOOST_CHECK_LE(t_int, t_int);
    BOOST_CHECK_GE(t_int, t_int);
    BOOST_CHECK_NE(t_int, t_double);

    BOOST_CHECK_LE(t_double, t_double);
    BOOST_CHECK_GE(t_double, t_double);
    BOOST_CHECK_NE(t_double, t_int);

    BOOST_CHECK(t_double < t_int || t_int < t_double);
    BOOST_CHECK(t_double > t_int || t_int > t_double);
}

BOOST_AUTO_TEST_CASE(hash_code_type_id)
{
    using namespace boost;
    std::size_t t_int1 = type_id<int>().hash_code();
    std::size_t t_double1 = type_id<double>().hash_code();

    std::size_t t_int2 = type_id<int>().hash_code();
    std::size_t t_double2 = type_id<double>().hash_code();

    BOOST_CHECK_EQUAL(t_int1, t_int2);
    BOOST_CHECK_NE(t_int1, t_double2);
    BOOST_CHECK_LE(t_double1, t_double2);
}

BOOST_AUTO_TEST_CASE(hash_code_template_id)
{
    using namespace boost;
    std::size_t t_int1 = template_id<int>().hash_code();
    std::size_t t_double1 = template_id<double>().hash_code();

    std::size_t t_int2 = template_id<int>().hash_code();
    std::size_t t_double2 = template_id<double>().hash_code();

    BOOST_CHECK_EQUAL(t_int1, t_int2);
    BOOST_CHECK_NE(t_int1, t_double2);
    BOOST_CHECK_LE(t_double1, t_double2);
}

template <class T1, class T2>
static void test_with_modofiers() {
    using namespace boost;

    template_index t1 = template_id_with_cvr<T1>();
    template_index t2 = template_id_with_cvr<T2>();

    BOOST_CHECK_NE(t2, t1);
    BOOST_CHECK(t1 < t2 || t2 < t1);
    BOOST_CHECK(t1 > t2 || t2 > t1);

    BOOST_CHECK_EQUAL(t1, template_id_with_cvr<T1>());
    BOOST_CHECK_EQUAL(t2, template_id_with_cvr<T2>());

    BOOST_CHECK_EQUAL(t1.hash_code(), template_id_with_cvr<T1>().hash_code());
    BOOST_CHECK_EQUAL(t2.hash_code(), template_id_with_cvr<T2>().hash_code());

    BOOST_CHECK_NE(t1.hash_code(), template_id_with_cvr<T2>().hash_code());
    BOOST_CHECK_NE(t2.hash_code(), template_id_with_cvr<T1>().hash_code());
}

BOOST_AUTO_TEST_CASE(template_id_storing_modifiers)
{
    test_with_modofiers<int, const int>();
    test_with_modofiers<int, const int&>();
    test_with_modofiers<int, int&>();
    test_with_modofiers<int, volatile int>();
    test_with_modofiers<int, volatile int&>();
    test_with_modofiers<int, const volatile int>();
    test_with_modofiers<int, const volatile int&>();

    test_with_modofiers<const int, int>();
    test_with_modofiers<const int, const int&>();
    test_with_modofiers<const int, int&>();
    test_with_modofiers<const int, volatile int>();
    test_with_modofiers<const int, volatile int&>();
    test_with_modofiers<const int, const volatile int>();
    test_with_modofiers<const int, const volatile int&>();

    test_with_modofiers<const int&, int>();
    test_with_modofiers<const int&, const int>();
    test_with_modofiers<const int&, int&>();
    test_with_modofiers<const int&, volatile int>();
    test_with_modofiers<const int&, volatile int&>();
    test_with_modofiers<const int&, const volatile int>();
    test_with_modofiers<const int&, const volatile int&>();

    test_with_modofiers<int&, const int>();
    test_with_modofiers<int&, const int&>();
    test_with_modofiers<int&, int>();
    test_with_modofiers<int&, volatile int>();
    test_with_modofiers<int&, volatile int&>();
    test_with_modofiers<int&, const volatile int>();
    test_with_modofiers<int&, const volatile int&>();
}

BOOST_AUTO_TEST_CASE(stream_operator_via_lexical_cast_testing)
{
    using namespace boost;
    std::string s_int1 = lexical_cast<std::string>(type_id<int>());
    BOOST_CHECK_EQUAL(s_int1, "int");

    std::string s_int2 = lexical_cast<std::string>(template_id<int>());
    BOOST_CHECK_EQUAL(s_int2, "int");

    std::string s_double1 = lexical_cast<std::string>(type_id<double>());
    BOOST_CHECK_EQUAL(s_double1, "double");
    
    std::string s_double2 = lexical_cast<std::string>(template_id<double>());
    BOOST_CHECK_EQUAL(s_double2, "double");
}

BOOST_AUTO_TEST_CASE(type_index_stripping_cvr_test)
{
    using namespace boost;

    BOOST_CHECK_EQUAL(type_id<int>(), type_id<const int>());
    BOOST_CHECK_EQUAL(type_id<int>(), type_id<const volatile int>());
    BOOST_CHECK_EQUAL(type_id<int>(), type_id<const volatile int&>());

    BOOST_CHECK_EQUAL(type_id<int>(), type_id<int&>());
    BOOST_CHECK_EQUAL(type_id<int>(), type_id<volatile int>());
    BOOST_CHECK_EQUAL(type_id<int>(), type_id<volatile int&>());


    BOOST_CHECK_EQUAL(type_id<double>(), type_id<const double>());
    BOOST_CHECK_EQUAL(type_id<double>(), type_id<const volatile double>());
    BOOST_CHECK_EQUAL(type_id<double>(), type_id<const volatile double&>());

    BOOST_CHECK_EQUAL(type_id<double>(), type_id<double&>());
    BOOST_CHECK_EQUAL(type_id<double>(), type_id<volatile double>());
    BOOST_CHECK_EQUAL(type_id<double>(), type_id<volatile double&>());
}

BOOST_AUTO_TEST_CASE(template_index_stripping_cvr_test)
{
    using namespace boost;

    BOOST_CHECK_EQUAL(template_id<int>(), template_id<const int>());
    BOOST_CHECK_EQUAL(template_id<int>(), template_id<const volatile int>());
    BOOST_CHECK_EQUAL(template_id<int>(), template_id<const volatile int&>());

    BOOST_CHECK_EQUAL(template_id<int>(), template_id<int&>());
    BOOST_CHECK_EQUAL(template_id<int>(), template_id<volatile int>());
    BOOST_CHECK_EQUAL(template_id<int>(), template_id<volatile int&>());


    BOOST_CHECK_EQUAL(template_id<double>(), template_id<const double>());
    BOOST_CHECK_EQUAL(template_id<double>(), template_id<const volatile double>());
    BOOST_CHECK_EQUAL(template_id<double>(), template_id<const volatile double&>());

    BOOST_CHECK_EQUAL(template_id<double>(), template_id<double&>());
    BOOST_CHECK_EQUAL(template_id<double>(), template_id<volatile double>());
    BOOST_CHECK_EQUAL(template_id<double>(), template_id<volatile double&>());
}


namespace my_namespace1 {
    class my_class{};
} 


namespace my_namespace2 {
    class my_class{};
} 


BOOST_AUTO_TEST_CASE(template_index_user_defined_class_test)
{
    using namespace boost;

    BOOST_CHECK_EQUAL(template_id<my_namespace1::my_class>(), template_id<my_namespace1::my_class>());
    BOOST_CHECK_EQUAL(template_id<my_namespace2::my_class>(), template_id<my_namespace2::my_class>());
    
    BOOST_CHECK_NE(template_id<my_namespace1::my_class>(), template_id<my_namespace2::my_class>());
    BOOST_CHECK_NE(
        template_id<my_namespace1::my_class>().name_demangled().find("my_namespace1::my_class"), 
        std::string::npos);

    BOOST_CHECK_EQUAL(type_id<my_namespace1::my_class>(), type_id<my_namespace1::my_class>());
    BOOST_CHECK_EQUAL(type_id<my_namespace2::my_class>(), type_id<my_namespace2::my_class>());
    
    BOOST_CHECK_NE(type_id<my_namespace1::my_class>(), type_id<my_namespace2::my_class>());
    BOOST_CHECK_NE(
        type_id<my_namespace1::my_class>().name_demangled().find("my_namespace1::my_class"), 
        std::string::npos);
}







