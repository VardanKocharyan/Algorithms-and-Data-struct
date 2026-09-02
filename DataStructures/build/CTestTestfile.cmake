# CMake generated Testfile for 
# Source directory: /home/vardan/Algorithms_and_DataStructures/DataStructures
# Build directory: /home/vardan/Algorithms_and_DataStructures/DataStructures/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_vector "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_vector")
set_tests_properties(test_vector PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
add_test(test_forward_list "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_forward_list")
set_tests_properties(test_forward_list PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
add_test(test_list "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_list")
set_tests_properties(test_list PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
add_test(test_deque "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_deque")
set_tests_properties(test_deque PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
add_test(test_stack "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_stack")
set_tests_properties(test_stack PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
add_test(test_queue "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_queue")
set_tests_properties(test_queue PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
add_test(test_priority_queue "/home/vardan/Algorithms_and_DataStructures/DataStructures/build/tests/test_priority_queue")
set_tests_properties(test_priority_queue PROPERTIES  _BACKTRACE_TRIPLES "/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;31;add_test;/home/vardan/Algorithms_and_DataStructures/DataStructures/CMakeLists.txt;0;")
subdirs("examples")
subdirs("tests")
