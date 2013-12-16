
#include <assert.h>
#include <thread>

#include "hayai.hpp"

#include "ring_buffer.hpp"

static const uint32_t kRingBufferSize = 1024;
static const int kNumEventsToGenerate = 20000000;

template<int64_t RingBufferSize>
static int TestConsume(processor::RingBuffer<int, RingBufferSize>* ring_buffer) {

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
    for (int64_t index = prev_sequence + 1; index <= next_sequence; index++) {
      int entry = *(ring_buffer->get(index));
      assert(entry == index);
      ++num_events_processed;
    }
    // Mark events consumed.
    ring_buffer->markConsumed(next_sequence);
    prev_sequence = next_sequence;
    if (num_events_processed == kNumEventsToGenerate)
      break;
  }
  return 0;
}

static int TestProduce() {
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
  return 0;
}

BENCHMARK(RingBuffer, ProduceAndConsumeEvents, 10, 10)
{
  TestProduce();
}