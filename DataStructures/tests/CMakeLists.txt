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

