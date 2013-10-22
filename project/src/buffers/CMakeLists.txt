SET (buffer_SRC buffer.cpp)

# buffer_LIB is defined in the top level Cmake file.
ADD_LIBRARY(${buffer_LIB} SHARED ${buffer_SRC})