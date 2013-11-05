#include "ring_buffer.hpp"

//TODO: Write a cross-platform pause function.
#include <xmmintrin.h>

namespace processor {

template<typename T>
RingBuffer<T>::RingBuffer(uint64_t events_size):
  events_size_(events_size),
  index_mask_(events_size - 1),
  publisher_sequence(0),
  consumer_sequence(0),
  events_(new T) {
  }

template<typename T>
RingBuffer<T>::~RingBuffer() {
  delete[] events_;
}

template<typename T>
T* RingBuffer<T>::get(uint64_t sequence) {
  return &events_[sequence & index_mask_];
}
// This will block till there is an empty slot to claim.
// To be always called by the producer.
template<typename T>
uint64_t RingBuffer<T>::next() {
  auto current_producer_sequence = publisher_sequence;
  auto next_producer_sequence = current_producer_sequence + 1;
  auto wrap_point = next_producer_sequence - events_size_;
  // TODO(Rajiv): Combine pausing with backoff + sleep.
  while (consumer_sequence.load(std::memory_order::memory_order_acquire) <= wrap_point) {
    _mm_pause();
  }
  return next_producer_sequence;
}

template<typename T>
void RingBuffer<T>::publish(uint64_t sequence) {
  publisher_sequence.store(sequence, std::memory_order::memory_order_release);
}

}  // processor