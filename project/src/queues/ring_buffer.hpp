#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <x86intrin.h>

#include <atomic>
#include <cinttypes>
#include <cstdio>

namespace processor {

static const int kCacheLineSize = 64;

// A simple ring buffer for single producers and single consumers.
// Does not support parallel consumers for now.
template<typename T, int64_t size>
class RingBuffer {

public:
  // Events_size must be a power of two.
  RingBuffer() :
    publisher_sequence_(-1),
    cached_consumer_sequence_(-1),
    events_size_(size),
    index_mask_(size - 1),
    consumer_sequence_(-1) {
  }

  // Used to get an event for a given sequence.
  //Can be called by both the producer and consumer.
  inline T* get(int64_t sequence) {
    return &events_[sequence & index_mask_];
  }

  // Can be called by either producer or consumer.
  inline int64_t getBufferSize() const {
    return events_size_;
  }

  // Called by the producer to get the next publish slot.
  // Will block till there is a slot to claim.
  int64_t nextProducerSequence() {
    int64_t current_producer_sequence = publisher_sequence_.load(std::memory_order::memory_order_relaxed);
    int64_t next_producer_sequence = current_producer_sequence + 1;
    int64_t wrap_point = next_producer_sequence - events_size_;
    //printf("\nCurrent seq: %" PRId64 ", next seq: %" PRId64 ", wrap_point: %" PRId64 "\n", current_producer_sequence, next_producer_sequence, wrap_point);
    // TODO(Rajiv): Combine pausing with backoff + sleep.
    if (cached_consumer_sequence_ > wrap_point) {
      return next_producer_sequence;
    }
    cached_consumer_sequence_ = getConsumerSequence();
    while (cached_consumer_sequence_ <= wrap_point) {
      _mm_pause();
      cached_consumer_sequence_ = getConsumerSequence();
    }
    return next_producer_sequence;
  }

  // Called by the producer to see what entries the consumer is done with.
  inline int64_t getConsumerSequence() const {
    return consumer_sequence_.load(std::memory_order::memory_order_acquire);
  }

  // Called by the producer after it has set the correct event entries.
  inline void publish(int64_t sequence) {
    publisher_sequence_.store(sequence, std::memory_order::memory_order_release);
  }

  // Called by the consumer to see where the producer is at.
  inline int64_t getProducerSequence()const {
    return publisher_sequence_.load(std::memory_order::memory_order_acquire);
  }

  // Called by the consumer to set the latest consumed sequence.
  inline void markConsumed(int64_t sequence) {
    // Assert if sequence is higher than the previous one?
    consumer_sequence_.store(sequence, std::memory_order::memory_order_release);
  }

  ~RingBuffer() {
  }

private:
  char cache_line_pad_1_[kCacheLineSize];
  std::atomic<int64_t> publisher_sequence_;
  int64_t cached_consumer_sequence_;
  T events_[size];
  int64_t events_size_;
  int64_t index_mask_;
  char cache_line_pad_2_[kCacheLineSize];
  std::atomic<int64_t> consumer_sequence_;
  char cache_line_pad_3_[kCacheLineSize];

};

}  // processor

#endif