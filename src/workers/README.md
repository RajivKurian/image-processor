Workers
=======

Used to do the actual image processing work.

1.  Runs one or more serial image-processing stages. The output of one stage is the input to the other.
2.  Use [Halide](https://github.com/halide/Halide) for the image-processing kernels.
3.  Extend to GPU. Halid GPU backend may not be the most mature.

Future improvements/investigation:

1.  Consider the GPU as yet another worker. Build data structures for queueing between cpu producers and gpu consumers. Use cuda streams, atomics and barriers as building-blocksfor the implementation.