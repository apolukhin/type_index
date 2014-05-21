//
// Copyright Antony Polukhin, 2014.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Unfortunately Boost.Test does not satisfies the requirements of Boost.TypeIndex library:
// * Boost.Test fails to work with RTTI off
// * release version of Boost.Test is not updated for a long time, so new fixes do not make their way to release

#ifndef BOOST_TEST_LIGHTWEIGHT_EMULATION
#define BOOST_TEST_LIGHTWEIGHT_EMULATION

#include <vector>
#include <utility>
#include <iostream>

namespace boost { namespace test_x2 {


class tester {
    typedef void (*test_function_t)();
    typedef std::pair<const char*, test_function_t> pair_t;
 
private:    
    std::vector<pair_t> tests_;
    int error_count_;

public:
    tester() : error_count_(0) {}

    void on_fail() {
        ++ error_count_;
    }

    void add_test(const char* name, test_function_t func) {
        tests_.push_back(pair_t(name, func));
    }

    int run() {
        std::cout << "Running " << tests_.size() << " tests\n";

        for (std::size_t i = 0; i < tests_.size(); ++i) {
            std::cout << "Running '" << tests_[i].first << "' test case\n";
            (*tests_[i].second)(); // calling function
        }

        return error_count_;
    }
};


extern tester g_tester;

}} // namespace boost::test_x2

#define BOOST_AUTO_TEST_CASE(name)                          \
    void name ();                                           \
    struct name ## _starter { name ## _starter () {         \
        boost::test_x2::g_tester.add_test(#name, & name);   \
    }} name ## _starter_variable;                           \
    void name ()                                            \
    /**/


#define BOOST_CHECK(pred)                                                                                   \
   do{                                                                                                      \
      if(!(pred)){                                                                                          \
         std::cout << __FILE__ << ":" << __LINE__ << ": Error in " << BOOST_STRINGIZE(pred) << std::endl;   \
         boost::test_x2::g_tester.on_fail();                                                                \
      }                                                                                                     \
   }while(0)

#define BOOST_CHECK_TRIPLE_IMPL(x, oper, y)                                                                 \
   do{                                                                                                      \
      if(!(x oper y)){                                                                                      \
         std::cout << __FILE__ << ":" << __LINE__ << ": Error in "                                          \
            << BOOST_STRINGIZE(x) << BOOST_STRINGIZE(oper) << BOOST_STRINGIZE(y)                            \
            << " where lhs = " << x << " and rhs = " << y << std::endl;                                     \
         boost::test_x2::g_tester.on_fail();                                                                \
      }                                                                                                     \
   }while(0)

#define BOOST_CHECK_EQUAL(x, y) BOOST_CHECK_TRIPLE_IMPL(x, ==, y)
#define BOOST_CHECK_NE(x, y) BOOST_CHECK_TRIPLE_IMPL(x, !=, y)
#define BOOST_CHECK_LE(x, y) BOOST_CHECK_TRIPLE_IMPL(x, <=, y)
#define BOOST_CHECK_GE(x, y) BOOST_CHECK_TRIPLE_IMPL(x, >=, y)

#endif // BOOST_TEST_LIGHTWEIGHT_EMULATION


#ifdef BOOST_TEST_MODULE

namespace boost { namespace test_x2 {
    tester g_tester;
}} // namespace boost::test_x2

int main() {
    return boost::test_x2::g_tester.run();
}

#endif


