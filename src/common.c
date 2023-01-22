#include "common.h"

const double SAMPLE_FREQUENCY = 0.5;
const double FLOAT_THRESHOLD = 0.0001;

unsigned short small_float(double flt) {
    return (-FLOAT_THRESHOLD < flt) && (flt < FLOAT_THRESHOLD);
}