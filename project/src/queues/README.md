Queues.
=============

Ideas shamelessly copied from the [Disruptor project](http://lmax-exchange.github.io/disruptor), [Martin Thompson](http://mechanical-sympathy.blogspot.com) and [Dmitry Vyukov](http://www.1024cores.net)

Used for ***Inter thread communication***. Two variants:

1.  **Ring-buffer:**
	1.  Standard ring-buffer with fixed 2^x entries.
	2.  Used mostly by work-producers to hand work to worker threads.
	3.  Entries might include pointers to other structures.

2.  **Variant:**
	1.  Used to transfer results from worker threads to aggregator.
	2.  No pointers, just a contiguous blob.
	3.  Contains a large buffer with a pointer for next write and last read positions.
	4.  Entries are allocated by taking a chunk from the next write - last read region.
	5.  When the consumer is done with a chunk the last-read position is updated.