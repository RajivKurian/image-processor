cmake_minimum_required (VERSION 2.6)
project (Image-Processor)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x -g -Wall ${CMAKE_CXX_FLAGS} ")

##########
# Src
##########
# For each subdirectory set the library name, add subdirectory and add header to includes.
#ADD_SUBDIRECTORY(src/aggregator)

# Buffers.
set (buffer_LIB buffer)
ADD_SUBDIRECTORY(src/buffers)
list (APPEND COMMON_INCLUDES src/buffers)

#ADD_SUBDIRECTORY(src/producers)
#ADD_SUBDIRECTORY(src/protocol)
#ADD_SUBDIRECTORY(src/queues)
#ADD_SUBDIRECTORY(src/workers)



#####################
# Testing with Gtest-1.7
#####################
ADD_SUBDIRECTORY(ext/gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


###########################
# Unit tests
##########################
# Add test file
include_directories(${COMMON_INCLUDES})
add_executable( runUnitTests tests/buffertest.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main ${buffer_LIB})
add_test( runUnitTests runUnitTests )