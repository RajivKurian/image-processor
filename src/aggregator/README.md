Aggregator
==========
Aggregates out of order requests from worker threads and ensures that responses are written out in order.

One type for each producer:

#Network:
##Uses a ring-buffer/queue per worker thread to get responses.
##First attempts to write responses synchronously. If it cannot then copies the remaining bytes, registers with the epoll/kqueue for write interest and writes them later.
##Always marks a buffer done before moving onto the other. Copies when it cannot be done synchronously. '''TODO''': Investigate this design.

#Memory mapped files
##Uses a ring-buffer/queue per worker thread to get responses.
###Since it writes to a mmaped file, writes should finish synchronously, unless the producer cannot keep up i.e. no space to write on the memory mapped-file.
