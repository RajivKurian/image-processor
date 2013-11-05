#include "ring_buffer.hpp"

#include <iostream>
#include <thread>

#include "gtest/gtest.h"

namespace ringbuffertest {

static const uint32_t kRingBufferSize = 4;

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
    std::cout << std::endl << "Test event created." << std::endl;
  }
  ~TestEvent() {
    delete[] fivechars_;
    std::cout << std::endl << "Test event destructed" << std::endl;
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
  int64_t next_sequence = -1;
  while (true) {
    next_sequence = ring_buffer->getProducerSequence();
    if (next_sequence > -1) break;
  }
  EXPECT_EQ(next_sequence, 0) << "Consumer did not get the next sequence";
  auto entry = ring_buffer->get(next_sequence);
  auto arr = entry->fivechars_;
  EXPECT_EQ(entry->num_, 1) << "Consumer did not see the right valye of num " << entry->num_;
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(arr[i], 'b') << "Consumer Entry not constructed properly found " << arr[i];
  }
  return 1;
}

TEST_F(RingBufferTest, HandlesProduction) {
  // Start the consumer.
  std::thread t{TestConsume, ring_buffer_};
  t.detach();

  // Producer.
  auto next_write_index = ring_buffer_->nextProducerSequence();
  EXPECT_EQ(next_write_index, 0) << "Ring buffer index is " << next_write_index << " should have been " << 1;
  auto entry = ring_buffer_->get(next_write_index);
  auto arr = entry->fivechars_;

  // First check to see the default event holds.
  EXPECT_EQ(entry->num_, 0) << "Entry not constructed properly num is " << entry->num_;
  // Change num to 1;
  entry->num_ = 1;
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(arr[i], 'a') << "Entry not constructed properly found " << arr[i];
    // Then change array to contain only 'b's.
    arr[i] = 'b';
  }
  // Then publish.
  ring_buffer_->publish(next_write_index);
}

} // ringbuffertest

