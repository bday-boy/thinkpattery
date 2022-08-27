#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>

// Uses modf to split a float of seconds into seconds and nanosencds
struct timespec decimal_to_timespec(double time_amount);

#endif // UTILS_H_