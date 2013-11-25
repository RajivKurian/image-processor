################
# Hayai benchmarks.
################
SET (ring_buffer_benchmark_SRC ring_buffer_benchmark.cpp)
SET(perf_main_EXECUTABLE perf)
ADD_EXECUTABLE(${perf_main_EXECUTABLE} perf.cpp ${ring_buffer_benchmark_SRC})



# Not needed now.
SET (ring_buffer_perf_SRC ring_buffer_perf.cpp)
SET (ring_buffer_perf_EXECUTABLE ring_buffer_perf)
ADD_EXECUTABLE(${ring_buffer_perf_EXECUTABLE} ${ring_buffer_perf_SRC})