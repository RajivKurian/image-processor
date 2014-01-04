SET (thread_utils_SRC thread_utils.cpp)

# thread_utils_LIB is defined in the top level Cmake file.
SET(OperatingSystem "Mac OS X")

IF(NOT ${OperatingSystem} MATCHES "Mac OS X") 
  ADD_LIBRARY(${thread_utils_LIB} SHARED ${thread_utils_SRC})
ENDIF(NOT ${OperatingSystem} MATCHES "Mac OS X")