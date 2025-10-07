# Copyright  (c) 2016-2025 Antony Polukhin
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

include(BoostTest OPTIONAL RESULT_VARIABLE HAVE_BOOST_TEST)

if(NOT HAVE_BOOST_TEST)
  return()
endif()

set(BOOST_TEST_LINK_LIBRARIES Boost::type_index Boost::core)

boost_test(TYPE run SOURCES type_index_test.cpp)
boost_test(TYPE run SOURCES type_index_runtime_cast_test.cpp LINK_LIBRARIES Boost::smart_ptr)
boost_test(TYPE run SOURCES type_index_constexpr_test.cpp)
boost_test(TYPE run SOURCES type_index_test.cpp COMPILE_OPTIONS -fno-rtti COMPILE_DEFINITIONS BOOST_NO_RTTI=1 NAME type_index_test_no_rtti)
boost_test(TYPE run SOURCES ctti_print_name.cpp)

# TODO:
#    [ run testing_crossmodule.cpp test_lib_rtti ]
#    [ run testing_crossmodule.cpp test_lib_nortti : : : <rtti>off $(norttidefines) : testing_crossmodule_no_rtti ]
#    [ run testing_crossmodule_anonymous.cpp test_lib_anonymous_rtti : : : <test-info>always_show_run_output ]

boost_test(TYPE run SOURCES compare_ctti_stl.cpp)
boost_test(TYPE run SOURCES track_13621.cpp)

boost_test(TYPE compile-fail SOURCES type_index_test_ctti_copy_fail.cpp)
boost_test(TYPE compile-fail SOURCES type_index_test_ctti_construct_fail.cpp)
boost_test(TYPE compile SOURCES type_index_test_ctti_alignment.cpp)

    # Mixing RTTI on and off

# TODO:
    # MSVC sometimes overrides the /GR-, without `detect_missmatch` this test may link.
    # TODO: Disabled on MSVC. Enable again when there'll be an understanding of how to write this test correctly wor MSVC.
#    [ link-fail testing_crossmodule.cpp test_lib_rtti : $(nortti)  <toolset>msvc:<build>no : link_fail_nortti_rtti ]
#    [ link-fail testing_crossmodule.cpp test_lib_nortti : <toolset>msvc:<build>no : link_fail_rtti_nortti ]

 #   [ run testing_crossmodule.cpp test_lib_rtti_compat : : : $(nortti) $(compat) : testing_crossmodule_nortti_rtti_compat ]
#    [ run testing_crossmodule.cpp test_lib_nortti_compat : : : $(compat) : testing_crossmodule_rtti_nortti_compat ]

file(GLOB EXAMPLE_FILES "../examples/*.cpp")
foreach (testsourcefile ${EXAMPLE_FILES})
    boost_test(TYPE run SOURCES ${testsourcefile} INCLUDE_DIRECTORIES ../examples LINK_LIBRARIES Boost::unordered)

    get_filename_component(testname ${testsourcefile} NAME_WE)
    if(NOT testname STREQUAL "table_of_names")
        boost_test(TYPE run SOURCES ${testsourcefile} NAME ${testname}_no_rtti COMPILE_OPTIONS -fno-rtti COMPILE_DEFINITIONS BOOST_NO_RTTI=1 INCLUDE_DIRECTORIES ../examples LINK_LIBRARIES Boost::unordered)
    endif()
endforeach()
