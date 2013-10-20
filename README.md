Image-processor
===============

A multi-threaded image processing application that:

1.  Allows clients to create pipelines from a collection of image-processing operations.
2.  Clients then send in requests that include a pipeline id and an image.

Building
-------
  Make sure you have cmake. apt-get (debian) and port(OS X) both have repos.

    git clone https://github.com/RajivKurian/image-processor.git
    cd image-processor
    mkdir build
    cd build
    cmake ../src
    make

If you need Eclipse CDT files do something like:

cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_ECLIPSE_VERSION=4.3 ../src 

**Status:**  Just in the ideas phase now. Minimal real code.

Details
=====

1.  A single thread uses multiplexed IO (epoll/kqueue) to read requests from the network. It then divides a request into parallel operations that can proceed independently.
2.  Each parallel operation is sent to a worker thread for completion. Parallel operations from a single request could complete out of order. All results are sent to an aggregator thread for ordering.
3.  An aggregator thread puts results back in order and sends them to clients.
4. The image processing kernels will be written in [Halide](http://halide-lang.org) which will allow us to make use of SIMD instructions.

General principles
---------------

1.  Preallocate: In steady state there should be very few allocations. The ring buffer entries are preallocated. Buffers are allocated from a pool. This forces us to pick the right constants, but is good for performance.
2.  Single writer: Even though the app is multi-threaded, each thread has a fixed role. No two threads write to the same memory. This is good for performance and allows us to write clean, easy to debug single-threaded logic for the most part. Non-blocking queues which enforce the single writer principle are used to communicate between threads. This rule is also applied to memory allocations. If a buffer is allocated in a thread, then it is also freed by that thread.
3.  Try to minimize data copy: Image data can be pretty big, hence copies are expensive.

Future Extensions
---------------

 We could model the GPU as an independent worker. Cuda streams, atomics and barriers could help us do this. Image processing operations would have to be written for the GPU too. Halide already has a GPU backend so maybe we can repurpose it and use the same kernels.



