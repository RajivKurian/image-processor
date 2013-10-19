Producers
===========

Producers produce the workload for the image-processor worker-threads. In general producer work-flow looks like this:

1.  Parse requests ensuring they are correctly formed.
2.  Break a request into possible parallel requests.
3.  Assign requests to worker threads using a ring-buffer.
4.  The API will be slightly different based on the producer. Try to unify by using number of arguments for variability.

**Note:** Producers do not write responses back to clients. That work is done by aggregators.

Producers in order of priority of completion:

1.  **Network producers:**
	1.  Multiplex client connections using epoll/kqueue. No other backends will be supported for now. Epoll is first priority since I do not have a OS X/ BSD system.
	2.  Aggregator will write responses to the client sockets.

2. **IPC producers:**
	1.  This will be our interface for other languages.
	2.  This is also a consumer since the "real" producer will be built in another language.
	3.  Will only support a single client thread. Clients are responsible for mutexes etc for multi-thread access.
	4.  Will use a pair of memory-mapped files for implementation.