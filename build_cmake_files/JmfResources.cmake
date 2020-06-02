cmake_minimum_required(VERSION 3.13)

include(CMakeParseArguments)
include(JmfCommon)

function(ADD_JMF_RESOURCE)
  cmake_parse_arguments(ADD_JMF_RESOURCE "" "" "MODULE_NAME" ${ARGN})
  install(${ADD_JMF_RESOURCE_UNPARSED_ARGUMENTS})
endfunction(ADD_JMF_RESOURCE)
