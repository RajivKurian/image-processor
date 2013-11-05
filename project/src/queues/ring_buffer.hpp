#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>

#include <atomic>

namespace processor {

static const int kCacheLineSize = 64;

// A simple ring buffer for single producers and single consumers.
// Inspired by the Disruptor.
// Encapsulates wait-strategy and claim strategy. No pluggability.

template<typename T>
class RingBuffer {

public:
  // Events_size must be a power of two.
  RingBuffer(uint64_t events_size);
  // Used to get an event for a given sequence.
  T* get(uint64_t sequence);
  uint64_t getBufferSize();
  // This will block till there is an empty slot to claim.
  uint64_t next();
  void publish(uint64_t sequence);
  ~RingBuffer();

private:
  char cache_line_pad_1_[kCacheLineSize];
  std::atomic<uint64_t> publisher_sequence;
  T* events_;
  uint64_t events_size_;
  uint64_t index_mask_;
  char cache_line_pad_2_[kCacheLineSize];
  std::atomic<uint64_t> consumer_sequence;
  char cache_line_pad_3_[kCacheLineSize];

};

}  // processor

#endif