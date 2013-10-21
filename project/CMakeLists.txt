cmake_minimum_required (VERSION 2.6)
project (Image-Processor)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x -g -Wall ${CMAKE_CXX_FLAGS} ")

##########
# Src
##########
#ADD_SUBDIRECTORY(aggregator)
ADD_SUBDIRECTORY(src/buffers)
#ADD_SUBDIRECTORY(producers)
#ADD_SUBDIRECTORY(protocol)
#ADD_SUBDIRECTORY(queues)
#ADD_SUBDIRECTORY(workers)


#####################
# Testing with Gtest-1.7
#####################
ADD_SUBDIRECTORY(ext/gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


###########################
# Unit tests
##########################
# Add test cpp file
add_executable( runUnitTests tests/mocktest.cpp )
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
add_test( runUnitTests runUnitTests )

