# TODO add Copyright

cmake_minimum_required(VERSION 3.10)

project(c_plusplus_studies)

set(CMAKE_CXX_STANDARD 17)
set(JMF_UNIT_TEST_PREFIX "utest-")

add_subdirectory(dev)
add_subdirectory(externals/third_party/googletest)
