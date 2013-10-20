cmake_minimum_required (VERSION 2.6)
project (Image-Processor)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} ")

#ADD_SUBDIRECTORY(aggregator)
ADD_SUBDIRECTORY(buffers)
#ADD_SUBDIRECTORY(producers)
#ADD_SUBDIRECTORY(protocol)
#ADD_SUBDIRECTORY(queues)
#ADD_SUBDIRECTORY(workers)
