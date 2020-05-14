# TODO add Copyright

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

include_directories(.)

set(SOURCES ${SOURCES})

add_executable(${JMF_MODULE_NAME} ${SOURCES})

add_library(${JMF_UNIT_TEST_PREFIX}${JMF_MODULE_NAME}-static STATIC ${SOURCES})
