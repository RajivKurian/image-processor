Protocol
=========

**A few concepts:**

**Stage:** A stage is a discrete image processing operation. For example blurring or resizing.

**Pipeline:** A pipeline is a composition of stages. For example blur an image then resize it to a given dimension. Stages in a pipeline are either parallel (independent) or serial (depend on each other).

The protocol needs to support:

1.  Getting all the image-processor stages (blur, resize etc) a server supports.
2.  Getting all the existing pipelines. 
3.  Creating a pipeline by composing these stages.
4.  Run an image through a pipeline.

Implementation:
=================

Use a length prefixed protocol so that appropriate contiguous buffers can be allocated to hold an entire request. For eg:

Length of request| Request

<----4 bytes---->| length * bytes

This will allow us to:
	1.  Read the first 4 bytes (provided we get to read at least 4) of a request into a stack allocated buffer. This will give us the length of the following request.
	2.  Once we know the length we can pick an appropriately sized free buffer to hold the request.

Draw inspiration from the Redis protocol.