#include "buffer.hpp"

#include <stdio.h>
#include "gtest/gtest.h"

namespace buffertest {

static const uint32_t buffer_size = 1024 * 1024;  // 1000 KB.

class BufferTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BufferTest() {
    // You can do set-up work for each test here.
    buffer_ = new processor::Buffer(buffer_size);
  }

  virtual ~BufferTest() {
    // You can do clean-up work that doesn't throw exceptions here.
    delete buffer_;
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
  processor::Buffer* buffer_;
};

TEST_F(BufferTest, HandlesCreation) {
  EXPECT_EQ(buffer_->getCapacity(), buffer_size) << "Buffer size does not match during creation.";
}

TEST_F(BufferTest, CheckInitialWritePosition) {
  EXPECT_EQ(buffer_->getCurrentWritePosition().write_pos,
    buffer_->getReadPosition()) << "Initial write pointer doesn't equal start position";
}

}  // buffertest