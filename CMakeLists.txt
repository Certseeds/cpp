cmake_minimum_required(VERSION 3.16.6)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 13)
set(PROJECT_VERSION_PATCH 0) # delete endsWith strs when master merge dev

project(cpp
        DESCRIPTION "my cpp workspace"
        HOMEPAGE_URL "https://github.com/Certseeds/cpp"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

string(TIMESTAMP time_str "%Y/%m/%d  %H:%M:%S")
MESSAGE(STATUS "Compiler In \n ${time_str}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLists.txt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/algorithm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/basic)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

enable_testing()

