Buffers
========

Buffer implementation.
#Support zero copy.
#Maintain write index.
#Will be attached to a epoll/kqueue structures to continue reading till a complete request is formed.
#Probably just use jemalloc to malloc and free underlying buffers since allocations and free happen on the same thread.