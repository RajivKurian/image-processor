#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <thread>

#include "thread_utils.hpp"

// Only handles pthread api for now.
void set_current_thread_affinity_and_exit_on_error(int core_number, const char* msg) {
  assert(core_number < hardware_concurrency());
  pthread_t thread = pthread_self();
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_number, &cpuset);
  int s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (s != 0)
    handle_error_en(s, "pthread_setaffinity_np consumer");
}

int hardware_concurrency() noexcept {
  return std::thread::hardware_concurrency();
}
