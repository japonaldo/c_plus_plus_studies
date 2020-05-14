# TODO add Copyright

get_filename_component(JMF_MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(JMF_TEST_MODULE_NAME ${JMF_UNIT_TEST_PREFIX}${JMF_MODULE_NAME})

include_directories(.)
include_directories(src)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${JMF_TEST_MODULE_NAME} ${TEST_SOURCES})

add_test(NAME ${JMF_TEST_MODULE_NAME} COMMAND ${JMF_TEST_MODULE_NAME})

#target_link_libraries(${JMF_TEST_MODULE_NAME} PUBLIC ${JMF_TEST_MODULE_NAME}-static gtest)
target_link_libraries(${JMF_TEST_MODULE_NAME} PUBLIC gtest gmock)