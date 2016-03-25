//
// Copyright Klemens Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/type_index/ctti_type_index.hpp>
#include <vector>


#include <boost/core/lightweight_test.hpp>

#include <iostream>
using namespace std;

#if ! defined( BOOST_NO_CXX11_VARIADIC_TEMPLATES ) && ! defined ( BOOST_NO_CXX11_CONSTEXPR ) && ! defined( BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION )

template<typename ...Args>
struct my_dummy_tuple {};

int main()
{
    using ti = boost::typeindex::ctti_type_index;
    using t1 = int;
    using t2 = double;
    using t3 = my_dummy_tuple<int, double>;
    using t4 = my_dummy_tuple<double, int, int, int, double, void*>;


    using ct1 = typename boost::typeindex::ctti_type_index_t<t1>;
    using ct2 = typename boost::typeindex::ctti_type_index_t<t2>;
    using ct3 = typename boost::typeindex::ctti_type_index_t<t3>;
    using ct4 = typename boost::typeindex::ctti_type_index_t<t4>;

    BOOST_STATIC_ASSERT( ct1() != ct2());
    BOOST_STATIC_ASSERT( ct1() == ct1());

    BOOST_STATIC_ASSERT( ct2() <  ct1());
    BOOST_STATIC_ASSERT( ct3() >  ct4());
    BOOST_STATIC_ASSERT( ct1() >= ct1());
    BOOST_STATIC_ASSERT( ct1() <= ct1());


    cout << ct1().to_string() << endl;
    cout << ct2().to_string() << endl;
    cout << ct3().to_string() << endl;
    cout << ct4().to_string() << endl;

    BOOST_TEST(to_string(ct1()) == ti::type_id<t1>().pretty_name());
    BOOST_TEST(to_string(ct2()) == ti::type_id<t2>().pretty_name());
    BOOST_TEST(to_string(ct3()) == ti::type_id<t3>().pretty_name());
    BOOST_TEST(to_string(ct4()) == ti::type_id<t4>().pretty_name());

    return boost::report_errors();
}

#else

int main()
{


    return 0;
}
#endif
