// Copyright 2013-2014 Antony Polukhin

// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying file LICENSE_1_0.txt
// or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)


//[type_index_names_example
/*`
    The following example shows how short (mangled) and human readable type names could be obtained from a type.
    Works with and without RTTI.
*/


#include <boost/type_index.hpp>
#include <iostream>
namespace bti = boost::typeind;

template <class T>
void foo(T) {
    std::cout << "\n Short name: " << bti::type_id<T>().raw_name();
    std::cout << "\n Readable name: " << bti::type_id<T>().pretty_name();
}

struct user_defined_type{};

int main() {
    // Call to
    foo(1);
    // will output something like this:
    //
    // (RTTI on)                            (RTTI off)
    // Short name: i                        Short name: int]
    // Readable name: int                   Readable name: int
    
    user_defined_type t;
    foo(t);
    // Will output:
    //
    // (RTTI on)                            (RTTI off)
    // Short name: 17user_defined_type      user_defined_type]
    // Readable name: user_defined_type     user_defined_type
}

/*`
    Short names are very compiler dependant: some compiler will output `.H`, others `i`.

    Readable names may also differ between compilers: `struct user_defined_type`, `user_defined_type`.
*/

//] [/type_index_names_example]
