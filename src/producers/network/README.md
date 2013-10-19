Network producers
=================

1.  Multiplex client connections using epoll/kqueue. No other backends will be supported for now.
2.  Epoll is first priority since I do not have a OS X/ BSD system.
3.  Aggregator will write responses to the client sockets.