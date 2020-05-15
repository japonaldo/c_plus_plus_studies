cmake_minimum_required(VERSION 3.13)

include(CMakeParseArguments)

#
# ADD_SUBDIRECTORY_IF_EXISTS(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
#
# Calls add_subdirectory() but only if source_dir exists and has a 
# CMakeLists.txt inside. It takes the same arguments as add_subdirectory()
function(ADD_SUBDIRECTORY_IF_EXISTS source_dir)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}/CMakeLists.txt)
    add_subdirectory(${source_dir} ${ARGN})
  endif()
endfunction()

#
# INCLUDE_IF_EXISTS(file)
# Calls include(file) only if file exists
function(INCLUDE_IF_EXISTS file)
  if(EXISTS ${file})
    include(${file})
  endif()
endfunction()

function(SET_IF_NOT_DEFINED VARIABLE VALUE)
  if(NOT DEFINED ${VARIABLE})
    set(${VARIABLE} ${VALUE} PARENT_SCOPE)
  endif()
endfunction()

#
# LIST_TO_STRING(OUTPUT_VAR input...)
#
# Converts a list (comma-separated) into a string (space-separated).
# Stores the result in variable OUTPUT_VAR.
function(LIST_TO_STRING OUTPUT_VAR input)
  list(APPEND input ${ARGN})
  set(str)
  string(REPLACE ";" " " str "${input}")
  set(${OUTPUT_VAR} "${str}" PARENT_SCOPE)
endfunction()
