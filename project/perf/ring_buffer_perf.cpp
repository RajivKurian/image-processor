#include "ring_buffer.hpp"

#include <assert.h>

#include <chrono>
#include <ctime>
#include <ratio>
#include <thread>

static const uint32_t kRingBufferSize = 1024;
static const int kNumEventsToGenerate = 2000000;

template<int RingBufferSize>
static int TestConsume(processor::RingBuffer<int, RingBufferSize>* ring_buffer) {
  using namespace std::chrono;
  typedef high_resolution_clock Clock;

  auto t1 = Clock::now();
  int64_t prev_sequence = -1;
  int64_t next_sequence = -1;
  int num_events_processed = 0;
  while (true) {
    // Spin till a new sequence is available.
    while (next_sequence <= prev_sequence) {
      _mm_pause();
      next_sequence = ring_buffer->getProducerSequence();
    }
    // Process everything in the batch.
    //printf("\nConsumer: Next sequence is %" PRId64 ", number of events to process is %" PRId64 "\n",
    //        next_sequence, next_sequence - prev_sequence);
    for (int64_t index = prev_sequence + 1; index <= next_sequence; index++) {
      //printf("\nConsumer: Processing sequence %" PRId64 " \n", index);
      int entry = *(ring_buffer->get(index));
      assert(entry == index);
      ++num_events_processed;
      if (num_events_processed == kNumEventsToGenerate)
        goto exit_consumer;
    }
    // Mark events consumed.
    ring_buffer->markConsumed(next_sequence);
    prev_sequence = next_sequence;
  }
exit_consumer:
  double time_span = (duration_cast<duration<double>>(Clock::now() - t1)).count();

  printf("\nTotal num events processed is %d in %f seconds\n", num_events_processed, time_span);
  printf("Events processing rate is: %f million events per second\n", num_events_processed/(time_span * 1000000));
  return 1;
}

int main() {
  processor::RingBuffer<int, kRingBufferSize>* ring_buffer = new processor::RingBuffer<int, kRingBufferSize>();
  // Start the consumer thread.
  // We must join later otherwise the application could exit while the consumer thread is still running.
  std::thread t{TestConsume<kRingBufferSize>, ring_buffer};

  for (int num_event = 0; num_event < kNumEventsToGenerate; num_event ++) {
    auto next_write_index = ring_buffer->nextProducerSequence();
    auto entry = ring_buffer->get(next_write_index);
    *entry = num_event;
    ring_buffer->publish(next_write_index);
  }
  t.join();
}