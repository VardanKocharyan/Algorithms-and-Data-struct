# Vector test executable
add_executable(test_vector sequence/test_vector.cpp)
target_link_libraries(test_vector PRIVATE mystl)

# Forward List test executable
add_executable(test_forward_list sequence/test_forward_list.cpp)
target_link_libraries(test_forward_list PRIVATE mystl)

# Add list test executable0
add_executable(test_list sequence/test_list.cpp)
target_link_libraries(test_list PRIVATE gtest gtest_main)
#target_include_directories(test_list PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_list PRIVATE mystl)

add_executable(test_deque sequence/test_deque.cpp)
target_link_libraries(test_deque PRIVATE gtest gtest_main mystl)

add_executable(test_stack adaptors/test_stack.cpp)
target_include_directories(test_stack PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_queue adaptors/test_queue.cpp)
target_include_directories(test_queue PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Priority Queue test executable
add_executable(test_priority_queue adaptors/test_priority_queue.cpp)
target_include_directories(test_priority_queue PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_priority_queue PRIVATE mystl)

# Unordered Map test executable
add_executable(test_unordered_map unordered/test_unordered_map.cpp)
target_link_libraries(test_unordered_map PRIVATE gtest gtest_main mystl)

# Unordered Set test executable
add_executable(test_unordered_set unordered/test_unordered_set.cpp)
target_link_libraries(test_unordered_set PRIVATE gtest gtest_main mystl)


