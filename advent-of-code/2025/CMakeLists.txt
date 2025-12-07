cmake_minimum_required(VERSION 3.16.6)
project(advent-of-code LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

enable_testing()
set(dependencies day1-1 day1-2 day2-1 day2-2 day3-1 day3-2)
LIST(APPEND dependencies day4-1 day4-2)
foreach (elementName IN LISTS dependencies)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${elementName})
endforeach ()
unset(dependencies)
