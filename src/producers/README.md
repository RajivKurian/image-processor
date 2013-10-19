Producers
===========

Producers produce the workload for the image-processor worker-threads. In general producers will work-flow looks like this:
#Parse requests ensuring they are correctly formed.
#Assign buffers from a buffer pool of custom buffer class used to hold API requests and images.
#Break a request into possible parallel requests.
#Assign requests to worker threads using our custom ring-buffer.
#Claim memory back from the worker-threads once they are done processing requests. The ring-buffer will provide hooks for this.
# The API will be slightly different based on the producer. Try to unify by using number of arguments for variability.

'''Note:''' Producers do not write responses back to clients. That work is done by aggregators.

Producers in order of priority:

# Network producers:
##Multiplex client connections using epoll/kqueue. No other backends will be supported for now. Epoll is first priority since I do not have a OS X/ BSD system.
##Aggregator will write responses to the client sockets.


#IPC producers:
##This will be our interface for other languages.
##This is also a consumer since the "real" producer will be built in another language.
##Will only support a single client thread. Clients are responsible for mutexes etc for multi-thread access.
##Will use a pair of memory-mapped files for implementation. The following needs to be addressed in the protocol:
###The app needs to know when it can re-use the response memory-mapped file reqions. The client implementation will need to communicate this information with '''requests'''.
###The client needs to know when it can re-use the request memory-mapped file regions. The app implementation will need to communicate this information with '''responses'''.
