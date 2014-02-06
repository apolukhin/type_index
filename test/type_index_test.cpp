//
// Copyright Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE type_index_test_module
#include <boost/test/unit_test.hpp>

#include <boost/type_index.hpp>

#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>

namespace my_namespace1 {
    class my_class{};
}


namespace my_namespace2 {
    class my_class{};
}


BOOST_AUTO_TEST_CASE(names_matches_type_id)
{
    using namespace boost::typeind;
    BOOST_CHECK_EQUAL(type_id<int>().pretty_name(), "int");
    BOOST_CHECK_EQUAL(type_id<double>().pretty_name(), "double");

    BOOST_CHECK_EQUAL(type_id<int>().name(),    type_id<int>().name());
    BOOST_CHECK_NE(type_id<int>().name(),       type_id<double>().name());
    BOOST_CHECK_NE(type_id<double>().name(),    type_id<int>().name());
    BOOST_CHECK_EQUAL(type_id<double>().name(), type_id<double>().name());
}

BOOST_AUTO_TEST_CASE(comparators_type_id)
{
    using namespace boost::typeind;
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

BOOST_AUTO_TEST_CASE(hash_code_type_id)
{
    using namespace boost::typeind;
    std::size_t t_int1 = type_id<int>().hash_code();
    std::size_t t_double1 = type_id<double>().hash_code();

    std::size_t t_int2 = type_id<int>().hash_code();
    std::size_t t_double2 = type_id<double>().hash_code();

    BOOST_CHECK_EQUAL(t_int1, t_int2);
    BOOST_CHECK_NE(t_int1, t_double2);
    BOOST_CHECK_LE(t_double1, t_double2);
}

template <class T1, class T2>
static void test_with_modofiers() {
    using namespace boost::typeind;

    type_index t1 = type_id_with_cvr<T1>();
    type_index t2 = type_id_with_cvr<T2>();

    BOOST_CHECK_NE(t2, t1);
    BOOST_CHECK(t1 < t2 || t2 < t1);
    BOOST_CHECK(t1 > t2 || t2 > t1);

    BOOST_CHECK_EQUAL(t1, type_id_with_cvr<T1>());
    BOOST_CHECK_EQUAL(t2, type_id_with_cvr<T2>());

    BOOST_CHECK_EQUAL(t1.hash_code(), type_id_with_cvr<T1>().hash_code());
    BOOST_CHECK_EQUAL(t2.hash_code(), type_id_with_cvr<T2>().hash_code());

    BOOST_CHECK_NE(t1.hash_code(), type_id_with_cvr<T2>().hash_code());
    BOOST_CHECK_NE(t2.hash_code(), type_id_with_cvr<T1>().hash_code());
}

BOOST_AUTO_TEST_CASE(type_id_storing_modifiers)
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

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    test_with_modofiers<int&&, const int>();
    test_with_modofiers<int&&, const int&>();
    test_with_modofiers<int&&, const int&&>();
    test_with_modofiers<int&&, int>();
    test_with_modofiers<int&&, volatile int>();
    test_with_modofiers<int&&, volatile int&>();
    test_with_modofiers<int&&, volatile int&&>();
    test_with_modofiers<int&&, const volatile int>();
    test_with_modofiers<int&&, const volatile int&>();
    test_with_modofiers<int&&, const volatile int&&>();
#endif
}

template <class T>
static void test_storing_nonstoring_modifiers_templ() {
    using namespace boost::typeind;

    type_index t1 = type_id_with_cvr<T>();
    type_index t2 = type_id<T>();

    BOOST_CHECK_EQUAL(t2, t1);
    BOOST_CHECK_EQUAL(t1, t2);
    BOOST_CHECK(t1 <= t2);
    BOOST_CHECK(t1 >= t2);
    BOOST_CHECK(t2 <= t1);
    BOOST_CHECK(t2 >= t1);

    BOOST_CHECK_EQUAL(t2.pretty_name(), t1.pretty_name());
}

BOOST_AUTO_TEST_CASE(type_id_storing_modifiers_vs_nonstoring)
{
    test_storing_nonstoring_modifiers_templ<int>();
    test_storing_nonstoring_modifiers_templ<my_namespace1::my_class>();
    test_storing_nonstoring_modifiers_templ<my_namespace2::my_class>();

    boost::typeind::type_index t1 = boost::typeind::type_id_with_cvr<const int>();
    boost::typeind::type_index t2 = boost::typeind::type_id<int>();
    BOOST_CHECK_NE(t2, t1);
    BOOST_CHECK(t1.pretty_name() == "const int" || t1.pretty_name() == "int const");
}

BOOST_AUTO_TEST_CASE(type_index_stream_operator_via_lexical_cast_testing)
{
    using namespace boost::typeind;

    std::string s_int2 = boost::lexical_cast<std::string>(type_id<int>());
    BOOST_CHECK_EQUAL(s_int2, "int");

    std::string s_double2 = boost::lexical_cast<std::string>(type_id<double>());
    BOOST_CHECK_EQUAL(s_double2, "double");
}

BOOST_AUTO_TEST_CASE(type_index_stripping_cvr_test)
{
    using namespace boost::typeind;

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


BOOST_AUTO_TEST_CASE(type_index_user_defined_class_test)
{
    using namespace boost::typeind;

    BOOST_CHECK_EQUAL(type_id<my_namespace1::my_class>(), type_id<my_namespace1::my_class>());
    BOOST_CHECK_EQUAL(type_id<my_namespace2::my_class>(), type_id<my_namespace2::my_class>());

#ifndef BOOST_NO_RTTI
    BOOST_CHECK(type_id<my_namespace1::my_class>() == typeid(my_namespace1::my_class));
    BOOST_CHECK(type_id<my_namespace2::my_class>() == typeid(my_namespace2::my_class));
    BOOST_CHECK(typeid(my_namespace1::my_class) == type_id<my_namespace1::my_class>());
    BOOST_CHECK(typeid(my_namespace2::my_class) == type_id<my_namespace2::my_class>());
#endif

    BOOST_CHECK_NE(type_id<my_namespace1::my_class>(), type_id<my_namespace2::my_class>());
    BOOST_CHECK_NE(
        type_id<my_namespace1::my_class>().pretty_name().find("my_namespace1::my_class"),
        std::string::npos);
}


#ifndef BOOST_NO_RTTI

class A { public: virtual ~A(){} };
class B: public A{};
class C: public B {};

BOOST_AUTO_TEST_CASE(comparators_type_id_runtime)
{
    C c1;
    B b1;
    A* pc1 = &c1;
    A& rc1 = c1;
    A* pb1 = &b1;
    A& rb1 = b1;
    BOOST_CHECK(typeid(rc1) == typeid(*pc1));
    BOOST_CHECK(typeid(rb1) == typeid(*pb1));

    BOOST_CHECK(typeid(rc1) != typeid(*pb1));
    BOOST_CHECK(typeid(rb1) != typeid(*pc1));

    BOOST_CHECK(typeid(&rc1) == typeid(pb1));
    BOOST_CHECK(typeid(&rb1) == typeid(pc1));

    BOOST_CHECK_EQUAL(boost::typeind::type_id_runtime(rc1), boost::typeind::type_id_runtime(*pc1));
    BOOST_CHECK_EQUAL(boost::typeind::type_id_runtime(rb1), boost::typeind::type_id_runtime(*pb1));

    BOOST_CHECK_NE(boost::typeind::type_id_runtime(rc1), boost::typeind::type_id_runtime(*pb1));
    BOOST_CHECK_NE(boost::typeind::type_id_runtime(rb1), boost::typeind::type_id_runtime(*pc1));
    BOOST_CHECK_EQUAL(boost::typeind::type_id_runtime(&rc1), boost::typeind::type_id_runtime(pb1));
    BOOST_CHECK_EQUAL(boost::typeind::type_id_runtime(&rb1), boost::typeind::type_id_runtime(pc1));

    BOOST_CHECK(boost::typeind::type_id_runtime(rc1) == typeid(*pc1));
    BOOST_CHECK(boost::typeind::type_id_runtime(rb1) == typeid(*pb1));

    BOOST_CHECK(boost::typeind::type_id_runtime(rc1) != typeid(*pb1));
    BOOST_CHECK(boost::typeind::type_id_runtime(rb1) != typeid(*pc1));
    BOOST_CHECK(boost::typeind::type_id_runtime(&rc1) == typeid(pb1));
    BOOST_CHECK(boost::typeind::type_id_runtime(&rb1) == typeid(pc1));
}


BOOST_AUTO_TEST_CASE(comparators_type_id_vs_type_info)
{
    using namespace boost::typeind;
    type_index t_int = type_id<int>();

    BOOST_CHECK(t_int == typeid(int));
    BOOST_CHECK(typeid(int) == t_int);
    BOOST_CHECK(t_int <= typeid(int));
    BOOST_CHECK(typeid(int) <= t_int);
    BOOST_CHECK(t_int >= typeid(int));
    BOOST_CHECK(typeid(int) >= t_int);

    type_index t_double = type_id<double>();

    BOOST_CHECK(t_double == typeid(double));
    BOOST_CHECK(typeid(double) == t_double);
    BOOST_CHECK(t_double <= typeid(double));
    BOOST_CHECK(typeid(double) <= t_double);
    BOOST_CHECK(t_double >= typeid(double));
    BOOST_CHECK(typeid(double) >= t_double);

    if (t_double < t_int) {
        BOOST_CHECK(t_double < typeid(int));
        BOOST_CHECK(typeid(double) < t_int);
        BOOST_CHECK(typeid(int) > t_double);
        BOOST_CHECK(t_int > typeid(double));


        BOOST_CHECK(t_double <= typeid(int));
        BOOST_CHECK(typeid(double) <= t_int);
        BOOST_CHECK(typeid(int) >= t_double);
        BOOST_CHECK(t_int >= typeid(double));
    } else {
        BOOST_CHECK(t_double > typeid(int));
        BOOST_CHECK(typeid(double) > t_int);
        BOOST_CHECK(typeid(int) < t_double);
        BOOST_CHECK(t_int < typeid(double));


        BOOST_CHECK(t_double >= typeid(int));
        BOOST_CHECK(typeid(double) >= t_int);
        BOOST_CHECK(typeid(int) <= t_double);
        BOOST_CHECK(t_int <= typeid(double));
    }

}

#endif // BOOST_NO_RTTI


