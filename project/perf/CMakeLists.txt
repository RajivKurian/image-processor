################
# Hayai benchmarks.
################
SET (ring_buffer_benchmark_SRC ring_buffer_benchmark.cpp)
SET(perf_main_EXECUTABLE perf)
ADD_EXECUTABLE(${perf_main_EXECUTABLE} perf.cpp ${ring_buffer_benchmark_SRC})



# Not needed now.
# Clang has some problems with chrono. Compile only on GNU.
SET (ring_buffer_perf_SRC ring_buffer_perf.cpp)
SET (ring_buffer_perf_EXECUTABLE ring_buffer_perf)
IF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  ADD_EXECUTABLE(${ring_buffer_perf_EXECUTABLE} ${ring_buffer_perf_SRC})
ENDIF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
