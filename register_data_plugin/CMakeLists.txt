cmake_minimum_required(VERSION 3.0.0)
project(register_data_plugin VERSION 1.0.0)

add_executable(register_data_plugin register_data_plugin.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_test(NAME register_data_plugin_dump_usage COMMAND register_data_plugin)
set_tests_properties(register_data_plugin_dump_usage
  PROPERTIES PASS_REGULAR_EXPRESSION "USAGE:.*"
  )
