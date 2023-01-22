#include "common.h"

unsigned short small_float(double flt) {
    return (-0.0001 < flt) && (flt < 0.0001);
}