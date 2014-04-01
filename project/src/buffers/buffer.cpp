#include "buffer.hpp"

#include <stdio.h>

namespace processor {

Buffer::Buffer(uint32_t capacity) :
  capacity_(capacity),
  num_bytes_written_(0),
  start_(new uint8_t[capacity]) {
    printf("\n Created a buffer with %d capacity", capacity);
  }

Buffer::~Buffer() {
  printf("\nBuffer destructor called.\n");
  delete[] start_;
}

write_marker Buffer::getCurrentWritePosition() const noexcept {
  return write_marker{start_+ num_bytes_written_, capacity_ - num_bytes_written_};
}

uint32_t Buffer::getCapacity() const noexcept {
  return capacity_;
}

uint8_t Buffer::getNumBytes() const noexcept {
  return num_bytes_written_;
}

bool Buffer::updateWritePosition(uint32_t num_bytes_written) noexcept {
  if ((num_bytes_written_ + num_bytes_written) <= capacity_) {
    num_bytes_written_ += num_bytes_written;
    return true;
  }
  return false;
}

const uint8_t* Buffer::getReadPosition() const noexcept {
  return start_;
}

void Buffer::reset() noexcept {
  num_bytes_written_ = 0;
}

}  // image-processor
