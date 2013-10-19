Buffers
========

Zero copy buffer implementation.

1.  Support zero copy.
2.  Will be attached to a epoll/kqueue structures to continue reading till a complete request is formed.
3.  Probably just use jemalloc to malloc and free underlying buffers since allocations and free happen on the same thread.