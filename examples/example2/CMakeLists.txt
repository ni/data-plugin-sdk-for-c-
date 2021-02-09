cmake_minimum_required(VERSION 3.0.0)
project(example2 VERSION 0.1.0)

#########################################
# Information about your plugin used to create NI dataplugin URI file
set(CPP_PLUGIN_NAME ${PROJECT_NAME})
set(CPP_PLUGIN_EXTENSION "example2")
set(CPP_PLUGIN_DESCRIPTION "example2 C++ DataPlugin")
set(CPP_PLUGIN_FULLPATH_BUILD "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.dll")
set(CPP_PLUGIN_FULLPATH_INSTALL "C:/Program Files/National Instruments/Shared/USI/DataPlugins/${PROJECT_NAME}/${PROJECT_NAME}.dll")
file(TO_NATIVE_PATH ${CPP_PLUGIN_FULLPATH_INSTALL} CPP_PLUGIN_FULLPATH_INSTALL)
file(TO_NATIVE_PATH ${CPP_PLUGIN_FULLPATH_BUILD} CPP_PLUGIN_FULLPATH_BUILD)
configure_file("../../uri_templates/plugin_write_file.uri.build.in" ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.build.uri)
configure_file("../../uri_templates/plugin_write_file.uri.install.in" ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.uri)
#########################################

#########################################
# Compile plugin DLL
file(GLOB plugin_src "*.h" "*.cpp")
include_directories (../../include)
add_library(${PROJECT_NAME} SHARED ${plugin_src})
target_link_libraries(${PROJECT_NAME} "comsuppw.lib")
#########################################

#########################################
# Register plugin for debugging
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND register_data_plugin -r "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.build.uri"
)
add_dependencies(${PROJECT_NAME} register_data_plugin)
#########################################

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#########################################
# needed to create C++ DataPlugin installer
file(GLOB uri_file ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.uri)
file(GLOB register_data_plugin_exe ${register_data_plugin_BINARY_DIR}/*.exe)

install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})
install(FILES ${uri_file} DESTINATION ${PROJECT_NAME})
install(FILES ${register_data_plugin_exe} DESTINATION ${PROJECT_NAME})
#########################################