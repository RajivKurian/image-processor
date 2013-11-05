#include "ring_buffer.hpp"

#include <thread>

#include "gtest/gtest.h"

namespace ringbuffertest {

static const uint32_t kRingBufferSize = 4;
static const int kNumEventsToGenerate = 12;

class TestEvent {
public:
  int num_;
  char *fivechars_;
  TestEvent() :
  num_(0),
  fivechars_(new char[5]) {
    for (int i = 0; i < 5; i++) {
      fivechars_[i] = 'a';
    }
    printf("\nTest event created.\n");;
  }
  ~TestEvent() {
    delete[] fivechars_;
    printf("\nTest event destructed\n");
  }
};

class RingBufferTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  RingBufferTest() {
    // You can do set-up work for each test here.
    
  }

  virtual ~RingBufferTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    ring_buffer_ = new processor::RingBuffer<TestEvent>(kRingBufferSize);
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    delete ring_buffer_;
  }

  
  // Objects declared here can be used by all tests in the test case for Foo.
  processor::RingBuffer<TestEvent>* ring_buffer_;
};

TEST_F(RingBufferTest, HandlesCreation) {
  EXPECT_EQ(ring_buffer_->getBufferSize(), kRingBufferSize) << "Ring Buffer size does not match after creation";
}

static int TestConsume(processor::RingBuffer<TestEvent>* ring_buffer) {
  int64_t prev_sequence = -1;
  int64_t next_sequence = -1;
  int num_events_processed = 0;
  while (true) {
    // Spin till a new sequence is available.
    while (true) {
      next_sequence = ring_buffer->getProducerSequence();
      if (next_sequence > prev_sequence) break;
      _mm_pause();
    }
    // Process everything in the batch.
    printf("\nConsumer: Next sequence is %" PRId64 ", number of events to process is %" PRId64 "\n",
            next_sequence, next_sequence - prev_sequence);
    for (int64_t index = prev_sequence + 1; index <= next_sequence; index++) {
      printf("\nConsumer: Processing sequence %" PRId64 " \n", index);
      auto entry = ring_buffer->get(index);
      auto num = entry->num_;
      ++num_events_processed;
      if (num == -2) {
        printf("\nConsumer: Received exit signal exiting.\n");
        goto exit_consumer;
      } else {
        printf("\nConsumer: Received a proper entry.\n");
        auto arr = entry->fivechars_;
        EXPECT_EQ(num, index) << "Consumer: Did not see the right value of num " << num;
        for (int i = 0; i < 5; i++) {
          EXPECT_EQ(arr[i], 'b') << "Consumer: Entry not constructed properly found " << arr[i];
        }
      }
    }
    // Mark events consumed.
    ring_buffer->markConsumed(next_sequence);
    prev_sequence = next_sequence;
  }
exit_consumer:
  printf("\nTotal num events processed is %d\n", num_events_processed);
  return 1;
}

TEST_F(RingBufferTest, HandlesProductionAndConsumption) {

  // Start the consumer thread.
  // We must join later otherwise the application will exit while the consumer thread is still running.
  std::thread t{TestConsume, ring_buffer_};

  // Producer.
  printf("\nProducer: Number of events to generate %d\n", kNumEventsToGenerate);
  for (int num_event = 0; num_event < kNumEventsToGenerate; num_event ++) {
    auto next_write_index = ring_buffer_->nextProducerSequence();
    EXPECT_EQ(next_write_index, num_event) << "Producer: Ring buffer index is " << next_write_index << " should have been " << 1;
    auto entry = ring_buffer_->get(next_write_index);
    auto arr = entry->fivechars_;

    if (num_event == kNumEventsToGenerate - 1) {
      // Signal exit with a -2;
      entry->num_ = -2;
    } else {
      // Change num to 1.
      entry->num_ = num_event;
      for (int i = 0; i < 5; i++) {
        // Then change array to contain only 'b's.
        arr[i] = 'b';
      }
    }
    // Then publish.
    ring_buffer_->publish(next_write_index);
  }
  t.join();
}

} // ringbuffertest