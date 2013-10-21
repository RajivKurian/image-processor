Network producers
=================

1.  Multiplex client connections using epoll/kqueue. No other backends will be supported for now.
2.  Epoll is first priority since I do not have a OS X/ BSD system.
3.  We could use libuv/libev, but they take over the run loop. This is not a problem for the network producer though.
4.  If we do not use libuv/libev either find or write a good hashed wheel timer implementation. This will be needed to kill clients who attempt slowloris attacks. Slowloris attacks are especially harmful here because large buffers are pre-allocated in anticipation of requests. Not a priority now though.
5.  Aggregator will write responses to the client sockets. Aggregators may use libev/libuv only if they can still be in control of the run loop.