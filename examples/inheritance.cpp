// Copyright 2013-2014 Antony Polukhin

// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying file LICENSE_1_0.txt
// or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//[type_index_derived_example
/*`
    The following example shows that `boost::type_info` is able to store the real type, successfully getting through
    all the inheritances.

    Example works with RTTI only. Without RTTI support it won't compile, producing a compile-time error with message:
    "boost::type_id_rtti_only(T&) requires RTTI"
*/

#include <boost/type_index.hpp>
#include <iostream>

struct A { virtual ~A(){} };
struct B: public A {};
struct C: public B {};

void print_real_type(const A& a) {
    std::cout << boost::typeind::type_id_runtime(a).pretty_name() << '\n';
}

int main() {
    C c;
    const A& c_as_a = c;
    print_real_type(c_as_a);    // Outputs `struct C`
    print_real_type(B());       // Outputs `struct B`
}

//] [/type_index_derived_example]
