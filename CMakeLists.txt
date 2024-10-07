cmake_minimum_required(VERSION 3.20)
project(FilterLibrary VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(module_common.cmake)

set(CATCH_INSTALL_DOCS OFF)
set(CATCH_INSTALL_EXTRAS OFF)
set(BUILD_TESTING OFF)
add_subdirectory(JUCE)
add_subdirectory(client)
add_subdirectory(modules)

add_subdirectory(3dti_AudioToolkit)
#target_link_libraries(FilterLibrary 3dti_AudioToolkit)



