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

# Set path for mysofa installation
set(mysofa_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libmysofa/libmysofa_install")

# Find the mysofa package
find_package(mysofa CONFIG REQUIRED)

# Add common library
add_library(common INTERFACE
        modules/sofa_renderer/src/sofa_renderer/SofaDodecRenderer.cpp
        modules/sofa_renderer/src/sofa_renderer/SofaDodecRenderer.h
        modules/sofa_renderer/src/sofa_renderer/SofaFilter.h
        modules/sofa_renderer/src/sofa_renderer/SofaFilter.cpp
)

# Link mysofa library
target_link_libraries(common INTERFACE mysofa::mysofa-static)
link_libraries(common)

add_subdirectory(client)
add_subdirectory(modules)



