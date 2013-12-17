#ifndef THREAD_UTILS_H_
#define THREAD_UTILS_H_

#define handle_error_en(en, msg) \
   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void set_current_thread_affinity_and_exit_on_error(int core_number, const char* msg);
int hardware_concurrency();

#endif