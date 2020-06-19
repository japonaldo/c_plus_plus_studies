# TODO add Copyright
include(JmfResources)

get_filename_component(JMF_MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_subdirectory(src)

ADD_JMF_RESOURCE(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources
                 DESTINATION tests/examples/${JMF_MODULE_NAME}
                 MODULE_NAME ${JMF_MODULE_NAME}
)