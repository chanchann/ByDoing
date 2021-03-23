cmake_minimum_required(VERSION 3.10)
project(smart_ptr_examples)

set(CMAKE_CXX_STANDARD 11)

add_executable(test_auto_ptr test_auto_ptr.cc)
add_executable(test_scoped_ptr test_scoped_ptr.cc)
add_executable(test_unique_ptr test_unique_ptr.cc)
add_executable(test_unique_u test_unique_u.cc)


