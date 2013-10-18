#define TEST_LIB_SOURCE
#include "test_lib.hpp"

namespace user_defined_namespace {
    class user_defined{};
}

namespace test_lib {

boost::type_index get_integer() {
    return boost::type_id<int>();
}

boost::type_index get_user_defined_class() {
    return boost::type_id<user_defined_namespace::user_defined>();
}

boost::type_index get_const_integer() {
    return boost::type_id_with_cvr<const int>();
}

boost::type_index get_const_user_defined_class() {
    return boost::type_id_with_cvr<const user_defined_namespace::user_defined>();
}

}

