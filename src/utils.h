#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <string.h>
#include <time.h>

// Uses modf to split a float of seconds into seconds and nanosencds
struct timespec decimal_to_timespec(double time_amount);

unsigned short starts_with(char * str, const char * prefix, size_t length);

#endif // UTILS_H_