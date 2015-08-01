//
// Copyright Antony Polukhin, 2015.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/type_index/ctti_type_index.hpp>

#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <string>

#include <boost/core/lightweight_test.hpp>
#define BOOST_TEST_LE(x, y) BOOST_TEST(x <= y)
#define BOOST_TEST_GE(x, y) BOOST_TEST(x >= y)

const char* hello1 = "Hello word";
const char* hello1_end = hello1 + sizeof("Hello word");
const char* hello2 = "Hello word, pal!";
const char* hello2_end = hello2 + sizeof("Hello word, pal!");

void strcmp_same() {
    using boost::typeindex::detail::constexpr_strcmp;

    BOOST_TEST(
        constexpr_strcmp(hello1, hello1) == 0
    );

    BOOST_TEST(
        constexpr_strcmp(hello2, hello2) == 0
    );

    BOOST_TEST(
        constexpr_strcmp(hello1, hello2) != 0
    );

    BOOST_TEST(
        constexpr_strcmp(hello2, hello1) != 0
    );

    BOOST_TEST(
        (constexpr_strcmp(hello2, hello1) < 0)
        ==
        (std::strcmp(hello2, hello1) < 0)
    );

    BOOST_TEST(
        (constexpr_strcmp(hello1, hello2) < 0)
        ==
        (std::strcmp(hello1, hello2) < 0)
    );
}

void search_same() {
    using boost::typeindex::detail::constexpr_search;
    BOOST_TEST(
        constexpr_search(hello1, hello1_end, hello2, hello2_end) == std::search(hello1, hello1_end, hello2, hello2_end)
    );

    BOOST_TEST(
        constexpr_search(hello2, hello2_end, hello1, hello1_end) == std::search(hello2, hello2_end, hello1, hello1_end)
    );

    const char* word = "word";
    const char* word_end = word + sizeof("word") - 1;
    BOOST_TEST(
        constexpr_search(hello1, hello1_end, word, word_end) == std::search(hello1, hello1_end, word, word_end)
    );

    BOOST_TEST(
        constexpr_search(hello2, hello2_end, word, word_end) == std::search(hello2, hello2_end, word, word_end)
    );
}


#ifndef BOOST_NO_CXX14_CONSTEXPR
template <class T>
struct is_boost_namespace {
    constexpr char cb[5] = {'b', 'o', 'o', 's', 't'};
    constexpr boost::typeindex::ctti_type_index type = boost::typeindex::ctti_type_index::type_id<T>();
    constexpr bool value = (boost::typeindex::detail::constexpr_search(type.name(), type.name() + 5, cb, cb + 5) != cb + 5);
};
#endif


void constexpr_test() {
    using namespace boost::typeindex;

    BOOST_CXX14_CONSTEXPR ctti_type_index t_int0 = ctti_type_index::type_id<int>();
    BOOST_CXX14_CONSTEXPR ctti_type_index t_short0 = ctti_type_index::type_id<short>();
    BOOST_CXX14_CONSTEXPR ctti_type_index t_int1 = ctti_type_index::type_id<int>();
    BOOST_CXX14_CONSTEXPR ctti_type_index t_short1 = ctti_type_index::type_id<short>();

    BOOST_CXX14_CONSTEXPR bool same0 = (t_int0 == t_int1);
    BOOST_TEST(same0);

    BOOST_CXX14_CONSTEXPR bool same1 = (t_short1 == t_short0);
    BOOST_TEST(same1);

    BOOST_CXX14_CONSTEXPR bool same2 = (t_int1 == t_int1);
    BOOST_TEST(same2);

    BOOST_CXX14_CONSTEXPR bool same3 = (t_short0 == t_short0);
    BOOST_TEST(same3);

    BOOST_CXX14_CONSTEXPR bool same4 = !(t_short0 < t_short0 || t_short0 > t_short0);
    BOOST_TEST(same4);

    BOOST_CXX14_CONSTEXPR bool same5 = (t_short0 <= t_short0 && t_short0 >= t_short0);
    BOOST_TEST(same5);


    BOOST_CXX14_CONSTEXPR bool not_same0 = (t_int0 != t_short1);
    BOOST_TEST(not_same0);

    BOOST_CXX14_CONSTEXPR bool not_same1 = (t_int1 != t_short0);
    BOOST_TEST(not_same1);

    BOOST_CXX14_CONSTEXPR bool not_same2 = (t_int1 < t_short0 || t_int1 > t_short0);
    BOOST_TEST(not_same2);


    BOOST_CXX14_CONSTEXPR const char* int_name = t_int0.name();
    BOOST_TEST(int_name);

    BOOST_CXX14_CONSTEXPR const char* short_name = t_short0.name();
    BOOST_TEST(short_name);

#ifndef BOOST_NO_CXX14_CONSTEXPR
    constexpr bool in_namespace = is_boost_namespace<ctti_type_index>::value;
    BOOST_TEST(in_namespace);

    constexpr bool not_in_namespace = !is_boost_namespace<std::string>::value;
    BOOST_TEST(not_in_namespace);
#endif

}


int main() {
    strcmp_same();
    search_same();
    constexpr_test();
    return boost::report_errors();
}

