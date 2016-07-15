//
// Copyright Chris Glover, 2016.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/type_index/runtime_cast.hpp>

#include <boost/core/lightweight_test.hpp>


// Classes include a member variable "name" with the
// name of the class hard coded so we can be sure that
// the pointer offsets are all working, since we're doing
// a cast from void* at some point.

#define IMPLEMENT_CLASS(type_name) \
        type_name() : name( #type_name ) {} \
        std::string name;

struct base {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI
    IMPLEMENT_CLASS(base)
};

struct single_derived : base {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(base)
    IMPLEMENT_CLASS(single_derived)
};

struct base1 {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI
    IMPLEMENT_CLASS(base1)
};

struct base2 {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI
    IMPLEMENT_CLASS(base2)
};

struct multiple_derived : base1, base2 {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(base1, base2)
    IMPLEMENT_CLASS(multiple_derived)
};

struct baseV1 : virtual base {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(base)
    IMPLEMENT_CLASS(baseV1)
};

struct baseV2 : virtual base {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(base)
    IMPLEMENT_CLASS(baseV2)
};

struct multiple_virtual_derived : baseV1, baseV2 {
    BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(baseV1, baseV2)
    IMPLEMENT_CLASS(multiple_virtual_derived)
};

void no_base()
{
    using namespace boost::typeindex;
    base b;
    base* b2 = runtime_cast<base>(&b);
    BOOST_TEST_NE(b2, (base*)nullptr);
    BOOST_TEST_EQ(b2->name, "base");
}

void single_base()
{
    using namespace boost::typeindex;
    single_derived d;
    base* b = &d;
    single_derived* d2 = runtime_cast<single_derived>(b);
    BOOST_TEST_NE(d2, (single_derived*)nullptr);
    BOOST_TEST_EQ(d2->name, "single_derived");
}

void multiple_base()
{
    using namespace boost::typeindex;
    multiple_derived d;
    base1* b1 = &d;
    multiple_derived* d2 = runtime_cast<multiple_derived>(b1);
    BOOST_TEST_NE(d2, (multiple_derived*)nullptr);
    BOOST_TEST_EQ(d2->name, "multiple_derived");

    base2* b2 = runtime_cast<base2>(b1);
    BOOST_TEST_NE(b2, (base2*)nullptr);
    BOOST_TEST_EQ(b2->name, "base2");
}

void virtual_base()
{
    using namespace boost::typeindex;
    multiple_virtual_derived d;
    base* b = &d;
    multiple_virtual_derived* d2 = runtime_cast<multiple_virtual_derived>(b);
    baseV1* bv1 = runtime_cast<baseV1>(b);
    baseV2* bv2 = runtime_cast<baseV2>(b);

    BOOST_TEST_NE(d2, (multiple_virtual_derived*)nullptr);
    BOOST_TEST_EQ(d2->name, "multiple_virtual_derived");

    BOOST_TEST_NE(bv1, (baseV1*)nullptr);
    BOOST_TEST_EQ(bv1->name, "baseV1");

    BOOST_TEST_NE(bv2, (baseV2*)nullptr);
    BOOST_TEST_EQ(bv2->name, "baseV2");
}

int main() {
    no_base();
    single_derived();
    return boost::report_errors();
}

