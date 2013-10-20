image-processor
===============

An Image processing application.

**Status:**  Just in the ideas phase now. No real code.

A multi-threaded application that:

1.  Allows clients to create pipelines from a collection of image-processing operations.
2.  Clients then send in requests that include a pipeline id and an image.

Details
=====

1.  A single thread uses multiplexed IO (epoll/kqueue) to read requests from the network. It then divides a request into parallel operations that can proceed independently.
2.  Each parallel operation is sent to a worker thread for completion. Parallel operations could complete out of order. All results are sent to an aggregator thread for ordering.
3.  An aggregator thread puts results back in order and sends them to clients.

General principles
---------------

1.  Preallocate: In steady state there should be very few allocations. The ring buffer entries are preallocated. Buffers are allocated from a pool. This forces us to pick the right constants, but is essential for performance.
2.  Single writer: Even though the app is multi-threaded responsibility is divided between threads. No two threads write to the same memory. This is good for performance and allows us to write clean, easy to debug single-threaded logic for the most part. Non-blocking queues which enforce the single writer principle are used to communicate between threads. This rule is also applied to memory allocations. If a buffer is allocated in a thread, then it is also freed by that thread.
3.  Try to minimize data copy: Image data can be pretty big, hence copies are expensive. Use zero-copy buffers as much as possible.

