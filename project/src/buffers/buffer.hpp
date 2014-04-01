#ifndef BUFFER_H_
#define BUFFER_H_


#include <cstddef>
#include <cinttypes>


namespace processor {

// Used to convey the write position of a buffer.
struct write_marker {
public:
  uint8_t* write_pos;
  uint32_t remaining;

// Should be stack allocated.
private:
  void* operator new(size_t);          // standard new
  void* operator new(size_t, void*);   // placement new
  void* operator new[](size_t);        // array new
  void* operator new[](size_t, void*); // placement array new
};

struct read_marker {
public:
  uint8_t* start;
  uint32_t num_bytes_written;

// Should be stack allocated.
private:
  void* operator new(size_t);          // standard new
  void* operator new(size_t, void*);   // placement new
  void* operator new[](size_t);        // array new
  void* operator new[](size_t, void*); // placement array new
};

class Buffer {
public:
  explicit Buffer(uint32_t capacity);

  // Disable copy and assignment.
  Buffer(const Buffer&) = delete;
  Buffer& operator = (const Buffer &) = delete;

  const uint8_t* getReadPosition() const noexcept;
  uint8_t getNumBytes() const noexcept;
  // Called to resume writing to a buffer.
  write_marker getCurrentWritePosition() const noexcept;
  uint32_t getCapacity() const noexcept;
  bool updateWritePosition(uint32_t num_bytes_written) noexcept;
  void reset() noexcept;
  ~Buffer();

private:
  // No zero argument constructor.
  Buffer();
  uint32_t capacity_;
  uint32_t num_bytes_written_;
  uint8_t* start_;
};

}  // processor

#endif
