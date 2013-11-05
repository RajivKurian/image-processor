SET (ring_buffer_SRC ring_buffer.cpp)

# ring_buffer_LIB is defined in the top level Cmake file.
ADD_LIBRARY(${ring_buffer_LIB} SHARED ${ring_buffer_SRC})