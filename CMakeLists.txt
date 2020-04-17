cmake_minimum_required (VERSION 2.6)
project (lcxjson CXX)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall")
endif()

add_library(lcxjson lcxjson.cc)
add_library(faculty faculty.cc)
add_executable(test_faculty test_faculty.cc)
target_link_libraries(test_faculty lcxjson faculty)
