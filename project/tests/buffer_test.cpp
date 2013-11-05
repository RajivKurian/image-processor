#include "buffer.hpp"

#include <stdio.h>
#include "gtest/gtest.h"

namespace buffertest {

static const uint32_t kBufferSize = 10;
static const char kChars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
static const uint32_t kNumChars = sizeof(kChars) / sizeof(kChars[0]);

class BufferTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BufferTest() {
    // You can do set-up work for each test here.
    
  }

  virtual ~BufferTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    buffer_ = new processor::Buffer(kBufferSize);
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    delete buffer_;
  }

  
  // Objects declared here can be used by all tests in the test case for Foo.
  processor::Buffer* buffer_;
};

TEST_F(BufferTest, HandlesCreation) {
  EXPECT_EQ(buffer_->getCapacity(), kBufferSize) << "Buffer size does not match during creation.";
}

TEST_F(BufferTest, CheckInitialWritePosition) {
  EXPECT_EQ(buffer_->getCurrentWritePosition().write_pos,
    buffer_->getReadPosition()) << "Initial write pointer doesn't equal start position";
}

TEST_F(BufferTest, Write) {

  // Write 5 chars to the underlying buffer and update the write position.
  auto marker = buffer_->getCurrentWritePosition();
  EXPECT_EQ(marker.remaining, kBufferSize);
  memcpy(marker.write_pos, kChars, kNumChars - 1);
  buffer_->updateWritePosition(kNumChars - 1);

  // Fetch the start and read and match the characters.
  auto start = buffer_->getReadPosition();
  for (uint32_t i = 0; i < kNumChars - 1; i++)
    EXPECT_EQ(kChars[i], start[i]) << "First Start not expected at pos " << i << " " << start[i];

  // Fetch the current write position and write a single character.
  marker = buffer_->getCurrentWritePosition();
  EXPECT_EQ(kBufferSize - kNumChars + 1, marker.remaining) << "Remaining characters does not match";
  memcpy(marker.write_pos,kChars + (kNumChars - 1), 1); 
  buffer_->updateWritePosition(1);

  /*
  // Print the characters again.
  for (uint32_t i = 0; i < kNumChars; i++)
    printf("\nBuffer[%d] = %c", i, start[i]);
  */

  // Check to see if everything got written.
  start = buffer_->getReadPosition();
  for (uint32_t i = 0; i < kNumChars; i++)
    EXPECT_EQ(kChars[i], start[i]) << "Second Start not expected at pos " << i << " " << start[i];

  // Check to see if setting an illegal write position works.
  EXPECT_EQ(buffer_->updateWritePosition(marker.remaining + 1), false) << "Buffer accepted an illegal update position";
  printf("\nBuffer position after illegal update is %d\n", buffer_->getNumBytes());
}

}  // buffertest