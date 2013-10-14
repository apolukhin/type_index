// Copyright 2013 Antony Polukhin

// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying file LICENSE_1_0.txt
// or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)


//[type_index_names_example
//`The following example shows how short (mangled) and human readable type names could be obtained from a type.
// Works with and without RTTI.

#include <boost/type_index/type_index_minimal.hpp>
#include <iostream>

template <class T>
void foo(T) {
    std::cout << "\n Short name: " << boost::type_id<T>().name();
    std::cout << "\n Readable name: " << boost::type_id<T>().name_demangled();
}

struct user_defined_type{};

int main() {
    // Call to
    foo(1);
    // will output something like this:
    //
    // Short name: .H
    // Readable name: int
    
    user_defined_type t;
    foo(t);
    // Will output:
    //
    // Short name: .?AUuser_defined_type@@
    // Readable name: struct user_defined_type
}

// The example 


//] [/type_index_names_example]
