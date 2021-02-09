cmake_minimum_required(VERSION 3.0.0)
project(da_plugin_sdk VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(register_data_plugin)
add_subdirectory(tests/simple_read_test)
add_subdirectory(examples/example1)
add_subdirectory(examples/example2)
