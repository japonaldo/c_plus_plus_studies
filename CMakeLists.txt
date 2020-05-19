# TODO add Copyright

cmake_minimum_required(VERSION 3.10)

project(c_plusplus_studies)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build_cmake_files)
set(JMF_3RD_PARTY_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/third_party)

set(JMF_UNIT_TEST_PREFIX "utest-")

include(JmfCommon)

add_subdirectory(dev)
add_subdirectory(externals/third_party/googletest)
