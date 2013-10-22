#include "buffer.hpp"

#include <stdio.h>
#include "gtest/gtest.h"

TEST(buffer_test, creation) {
  const uint32_t buffer_size = 1024 * 1024;  // 1000 KB.
  auto buf = new processor::Buffer(buffer_size);
  delete  buf;
}
