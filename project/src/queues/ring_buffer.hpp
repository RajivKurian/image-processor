#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <x86intrin.h>

#include <atomic>
#include <cinttypes>
#include <cstdio>

#define CACHE_LINE_SIZE 64
namespace processor {

// Forward declare.
template <typename T, uint64_t>
class RingBuffer;

// Struct to return aligned ring-buffer and pass on any errors.
template<typename T, uint64_t event_size>
struct RingBufferResult {
  RingBuffer<T, event_size>* ring_buffer;
  int return_code;
};

// A simple ring buffer for single producers and single consumers.
// Does not support parallel consumers for now.

// Align ring buffers to cache lines on stack allocations or static allocations.
// posix_memalign or aligned_alloc still need to be used for heap allocations.
template<typename T, uint64_t event_size>
class alignas(CACHE_LINE_SIZE) RingBuffer {

public:

  // Create ring-buffer aligned to  a cache line.
  static RingBufferResult<T, event_size> createAlignedRingBuffer() {
    // TODO: Consider allocating extra memory and using std::aligned instead
    // of using the non-standard posix_memalign.
    void * buffer;
    // Check and return error!
    auto rc = posix_memalign(&buffer, CACHE_LINE_SIZE, sizeof(processor::RingBuffer<T, event_size>));
    auto ring_buffer = new (buffer) RingBuffer<T, event_size>();
    return RingBufferResult<T, event_size>{ring_buffer, rc};
  }

  // No copy constructor.
  RingBuffer(const RingBuffer&) = delete;
  RingBuffer& operator = (const RingBuffer &) = delete;

  // Used to get an event for a given sequence.
  //Can be called by both the producer and consumer.
  inline T* get(int64_t sequence) const noexcept {
    return &events_[sequence & (event_size - 1)];  // size - 1 is the mask here.
  }

  // Can be called by either producer or consumer.
  inline uint64_t getBufferSize() const noexcept {
    return event_size;
  }

  // Called by the producer to get the next publish slot.
  // Will block till there is a slot to claim.
  int64_t nextProducerSequence() noexcept {
    int64_t current_producer_sequence = publisher_sequence_.load(std::memory_order::memory_order_relaxed);
    int64_t next_producer_sequence = current_producer_sequence + 1;
    int64_t wrap_point = next_producer_sequence - event_size;
    //printf("\nCurrent seq: %" PRId64 ", next seq: %" PRId64 ", wrap_point: %" PRId64 "\n", current_producer_sequence, next_producer_sequence, wrap_point);
    if (cached_consumer_sequence_ > wrap_point) {
      return next_producer_sequence;
    }
    cached_consumer_sequence_ = getConsumerSequence();
    while (cached_consumer_sequence_ <= wrap_point) {
      // TODO(Rajiv): Combine pausing with backoff + sleep.
      _mm_pause();
      cached_consumer_sequence_ = getConsumerSequence();
    }
    return next_producer_sequence;
  }

  // Called by the producer to see what entries the consumer is done with.
  inline int64_t getConsumerSequence() const noexcept {
    return consumer_sequence_.load(std::memory_order::memory_order_acquire);
  }

  // Called by the producer after it has set the correct event entries.
  inline void publish(int64_t sequence) noexcept {
    publisher_sequence_.store(sequence, std::memory_order::memory_order_release);
  }

  // Called by the consumer to see where the producer is at.
  inline int64_t getProducerSequence() const noexcept{
    return publisher_sequence_.load(std::memory_order::memory_order_acquire);
  }

  // Called by the consumer to set the latest consumed sequence.
  inline void markConsumed(int64_t sequence) noexcept {
    // Assert if sequence is higher than the previous one?
    consumer_sequence_.store(sequence, std::memory_order::memory_order_release);
  }

  ~RingBuffer() {
    delete[] events_;
  }

//private:
    // Events_size must be a power of two.
  RingBuffer() :
    publisher_sequence_(-1),
    cached_consumer_sequence_(-1),
    events_(new T[event_size]),
    consumer_sequence_(-1) {
  }

  std::atomic<int64_t> publisher_sequence_ ;
  int64_t cached_consumer_sequence_;
  T* events_;
   // Ensure that the consumer sequence is on it's own cache line to prevent false sharing.
  std::atomic<int64_t> consumer_sequence_ alignas(CACHE_LINE_SIZE); //__attribute__ ((aligned (CACHE_LINE_SIZE)));

};

}  // processor

#endif