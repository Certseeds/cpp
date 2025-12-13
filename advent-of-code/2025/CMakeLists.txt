cmake_minimum_required(VERSION 3.16.6)
project(advent-of-code LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

enable_testing()
set(dependencies day01-1 day01-2 day02-1 day02-2 day03-1 day03-2)
LIST(APPEND dependencies day04-1 day04-2 day05-1 day05-2 day06-1 day06-2)
LIST(APPEND dependencies day07-1 day07-2 day08-1 day08-2 day09-1 day09-2)
LIST(APPEND dependencies day10-1 day10-2)
foreach (elementName IN LISTS dependencies)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${elementName})
endforeach ()
unset(dependencies)
