#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h> // For size_t
#include <time.h>   // For timespec

// Uses modf to split a float of seconds into seconds and nanosencds
struct timespec decimal_to_timespec(double time_amount);

// Checks if a string has a given prefix. Only compares prefix_length number of
// characters.
unsigned short starts_with(char * str, const char * prefix,
                           size_t prefix_length);

#endif // UTILS_H_