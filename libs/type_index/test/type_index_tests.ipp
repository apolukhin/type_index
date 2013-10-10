//
// Copyright Antony Polukhin, 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/type_index/type_index_impl.hpp>

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

#ifndef BOOST_NO_RTTI

BOOST_AUTO_TEST_CASE(comparators_type_id_vs_type_info)
{
    using namespace boost;
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

BOOST_AUTO_TEST_CASE(type_index_stream_operator_via_lexical_cast_testing)
{
    using namespace boost;
    std::string s_int1 = lexical_cast<std::string>(type_id<int>());
    BOOST_CHECK_EQUAL(s_int1, "int");
    
    std::string s_double1 = lexical_cast<std::string>(type_id<double>());
    BOOST_CHECK_EQUAL(s_double1, "double");
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



BOOST_AUTO_TEST_CASE(type_index_user_defined_class_test)
{
    using namespace boost;

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
        type_id<my_namespace1::my_class>().name_demangled().find("my_namespace1::my_class"),
        std::string::npos);
}


#ifndef BOOST_NO_RTTI

class A { public: virtual ~A(){} };
class B: public A{};
class C: public B {};

BOOST_AUTO_TEST_CASE(comparators_type_id_rtti_only)
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

    BOOST_CHECK_EQUAL(boost::type_id_rtti_only(rc1), boost::type_id_rtti_only(*pc1));
    BOOST_CHECK_EQUAL(boost::type_id_rtti_only(rb1), boost::type_id_rtti_only(*pb1));

    BOOST_CHECK_NE(boost::type_id_rtti_only(rc1), boost::type_id_rtti_only(*pb1));
    BOOST_CHECK_NE(boost::type_id_rtti_only(rb1), boost::type_id_rtti_only(*pc1));
    BOOST_CHECK_EQUAL(boost::type_id_rtti_only(&rc1), boost::type_id_rtti_only(pb1));
    BOOST_CHECK_EQUAL(boost::type_id_rtti_only(&rb1), boost::type_id_rtti_only(pc1));

    BOOST_CHECK(boost::type_id_rtti_only(rc1) == typeid(*pc1));
    BOOST_CHECK(boost::type_id_rtti_only(rb1) == typeid(*pb1));

    BOOST_CHECK(boost::type_id_rtti_only(rc1) != typeid(*pb1));
    BOOST_CHECK(boost::type_id_rtti_only(rb1) != typeid(*pc1));
    BOOST_CHECK(boost::type_id_rtti_only(&rc1) == typeid(pb1));
    BOOST_CHECK(boost::type_id_rtti_only(&rb1) == typeid(pc1));
}

#endif // BOOST_NO_RTTI


