#include <math.h>

#include "utils.h"

struct timespec decimal_to_timespec(double time_amount) {
    struct timespec tim = {0, 0};

    double integral;
    double fractional = modf(time_amount, &integral);

    tim.tv_sec = (time_t) integral;
    tim.tv_nsec = (long int) (fractional * 1000000000.0);

    return tim;
};

unsigned short starts_with(char * str, const char * prefix, size_t length) {
    return strncmp(str, prefix, length) == 0;
};