#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>
#include <x86intrin.h>

#include <atomic>
#include <iostream>

namespace processor {

static const int kCacheLineSize = 64;

// A simple ring buffer for single producers and single consumers.
// Inspired by the Disruptor.
// Encapsulates wait-strategy and claim strategy. No pluggability.

template<typename T>
class RingBuffer {

public:
  // Events_size must be a power of two.
  RingBuffer(int64_t events_size) :
    publisher_sequence_(-1),
    events_(new T[events_size]),
    events_size_(events_size),
    index_mask_(events_size - 1),
    consumer_sequence_(-1) {
  }

  // Used to get an event for a given sequence.
  //Can be called by both the producer and consumer.
  T* get(int64_t sequence) const {
    return &events_[sequence & index_mask_];
  }

  // Can be called by either producer or consumer.
  int64_t getBufferSize()  const {
    return events_size_;
  }

  // Called by the producer to get the next publish slot.
  // Will block till there is a slot to claim.
  int64_t nextProducerSequence() const {
    std:: cout << std::endl << "HEYYYY" << std::endl;
    int64_t current_producer_sequence = publisher_sequence_.load(std::memory_order::memory_order_relaxed);
    std::cout << "\ncurrent producer sequence is " << current_producer_sequence << "\n";
    int64_t next_producer_sequence = current_producer_sequence + 1;
    std::cout << "\nNext producer sequence is " << next_producer_sequence << "\nEvents size is " << events_size_ << "\n";
    int64_t wrap_point = next_producer_sequence - events_size_;
    std::cout << "\nwrap point is " << wrap_point << "\n";
    // TODO(Rajiv): Combine pausing with backoff + sleep.
    while (getConsumerSequence() <= wrap_point) {
      _mm_pause();
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
    delete[] events_;
  }

private:
  char cache_line_pad_1_[kCacheLineSize];
  std::atomic<int64_t> publisher_sequence_;
  T* events_;
  int64_t events_size_;
  int64_t index_mask_;
  char cache_line_pad_2_[kCacheLineSize];
  std::atomic<int64_t> consumer_sequence_;
  char cache_line_pad_3_[kCacheLineSize];

};

}  // processor

#endif