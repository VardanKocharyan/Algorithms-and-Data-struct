cmake_minimum_required(VERSION 3.14)
project(MySTL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CompilerFlags.cmake OPTIONAL)

add_library(mystl INTERFACE)

target_include_directories(mystl INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(examples)
add_subdirectory(tests)

enable_testing()

set(ALL_TESTS
    test_vector
    test_forward_list
    test_list
    test_deque
    test_stack
    test_queue
    test_priority_queue
)

foreach(test ${ALL_TESTS})
    add_test(NAME ${test} COMMAND ${test})
endforeach()
