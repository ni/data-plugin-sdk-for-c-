cmake_minimum_required(VERSION 3.0.0)
project(simple_read_test VERSION 0.1.0)

include_directories (../../include)
add_executable(simple_read_test simple_read_test.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_test(NAME simple_read_test_dump_usage COMMAND simple_read_test)
set_tests_properties(simple_read_test_dump_usage
  PROPERTIES PASS_REGULAR_EXPRESSION "USAGE:.*"
  )
