// Copyright 2013 Antony Polukhin

// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying file LICENSE_1_0.txt
// or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//[type_index_registry_example
/*`
    The following example shows how an information about a type could be stored.
    Example works with and without RTTI.
*/

#include <boost/type_index/type_index_minimal.hpp>
#include <boost/unordered_set.hpp>
#include <cassert>

int main() {
    boost::unordered_set<boost::type_index> types;
    
    // Storing some `boost::type_info`s
    types.insert(boost::type_id<int>());
    types.insert(boost::type_id<float>());
    
    // `types` variable contains two `boost::type_index`es:
    assert(types.size() == 2);

    // Const, volatile and reference will be striped from the type:
    bool is_inserted = types.insert(boost::type_id<const int>()).second;
    assert(!is_inserted);
    assert(types.erase(boost::type_id<float&>()) == 1);
    
    // We have erased the `float` type, only `int` remains
    assert(*types.begin() == boost::type_id<int>());
}

//] [/type_index_registry_example]
