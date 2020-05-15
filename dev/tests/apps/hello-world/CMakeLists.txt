# TODO add Copyright

get_filename_component(JMF_MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(JmfMacroExecutable)
include(JmfMacroLibModule)

set(JMF_TEST_MODULE_NAME ${JMF_UNIT_TEST_PREFIX}${JMF_MODULE_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.cpp)

ADD_JMF_EXECUTABLE(
    NAME               ${JMF_TEST_MODULE_NAME}
    SRC                ${SOURCES}
    SYS_INCLUDE_DIRS   SYSTEM
    INCLUDE_DIRS       ${CMAKE_CURRENT_SOURCE_DIR}/src
                       ${JMF_APP_DIR}/hello-world/src
    LINKER_FLAGS  
    DEPS               gtest ${JMF_TEST_MODULE_NAME}-static
    )
    
add_test(NAME ${JMF_TEST_MODULE_NAME} COMMAND ${JMF_TEST_MODULE_NAME})