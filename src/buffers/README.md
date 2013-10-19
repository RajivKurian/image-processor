Buffers
========

Buffer implementation.

1.  Support zero copy.
2.  Maintain write index.
3.  Will be attached to a epoll/kqueue structures to continue reading till a complete request is formed.
4.  Probably just use jemalloc to malloc and free underlying buffers since allocations and free happen on the same thread.