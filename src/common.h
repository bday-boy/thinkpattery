#ifndef COMMON_H_
#define COMMON_H_

// Doesn't need to be very low -- the energies shown in each BATX/energy
// file usually only change every few seconds or so anyway.
extern const double SAMPLE_FREQUENCY;

// Threshold for avoiding floats that are too small.
extern const double FLOAT_THRESHOLD;

unsigned short small_float(double flt);

#endif