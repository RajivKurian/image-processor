SET (thread_utils_SRC thread_utils.cpp)

# thread_utils_LIB is defined in the top level Cmake file.
ADD_LIBRARY(${thread_utils_LIB} SHARED ${thread_utils_SRC})