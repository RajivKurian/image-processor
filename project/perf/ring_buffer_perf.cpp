//#include "gperftools/profiler.h"

#include "ring_buffer.hpp"

#include <iostream>
#include <assert.h>

#include <chrono>
#include <cstddef>
#include <ctime>
#include <ratio>
#include <thread>

#include "colors.h"
#include "thread_utils.hpp"

static const uint64_t kRingBufferSize = 1024;
static const int kNumEventsToGenerate = 200000000;

template<int RingBufferSize>
static int TestConsume(processor::RingBuffer<int, RingBufferSize>* ring_buffer) {
  using namespace std::chrono;
  typedef high_resolution_clock Clock;

  //set_current_thread_affinity_and_exit_on_error(1, "Consumer set affinity failed.");
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
    for (int64_t index = prev_sequence + 1; index <= next_sequence; index++) {
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
  printf("Events processing rate is: " ANSI_COLOR_GREEN "%f" ANSI_COLOR_RESET " million events per second\n", num_events_processed/(time_span * 1000000));
  return 1;
}

int main() {
  printf (ANSI_COLOR_BLUE "\nRingBuffer Performance Test" ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_GREEN "Number of concurrent threads supported is %d\n" ANSI_COLOR_RESET, hardware_concurrency());
  //set_current_thread_affinity_and_exit_on_error(0, "Producer set affinity failed.");

  // Align to a cache line.
  void* buffer;
  if (posix_memalign(&buffer, 64, sizeof(processor::RingBuffer<int, kRingBufferSize>)) != 0) {
    perror("posix_memalign did not work!");
    abort();
  }

  // Use a placement new on the aligned buffer.
  auto ring_buffer = new (buffer) processor::RingBuffer<int, kRingBufferSize>();
  std::cout << "Size of ring buffer is " << sizeof(processor::RingBuffer<int64_t, kRingBufferSize>) << " alignment of ring buffer " <<alignof(processor::RingBuffer<int, kRingBufferSize>) << ".\n";
  std::cout << "Address of ring buffer is " << std::hex << ring_buffer << ". Address of ring buffer consumer sequence is " << std::hex << &(ring_buffer->consumer_sequence_) << std::endl;

  // Start the consumer thread.
  // We must join later otherwise the application could exit while the consumer thread is still running.
  //ProfilerStart("/home/rajiv/cpp-projects/image-processor/build/sample.prof");
  std::thread t{TestConsume<kRingBufferSize>, ring_buffer};

  for (int num_event = 0; num_event < kNumEventsToGenerate; num_event ++) {
    auto next_write_index = ring_buffer->nextProducerSequence();
    auto entry = ring_buffer->get(next_write_index);
    *entry = num_event;
    ring_buffer->publish(next_write_index);
  }
  t.join();
  //ProfilerStop();
}