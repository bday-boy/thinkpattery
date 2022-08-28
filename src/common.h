#ifndef COMMON_H_
#define COMMON_H_

// Doesn't need to be very low -- the energies shown in each BATX/energy
// file usually only change every few seconds or so anyway.
#define SAMPLE_FREQUENCY 2.0 // float in seconds

// Threshold for avoiding floats that are too small.
#define FLOAT_THRESHOLD 0.0001

// Macro for avoiding division by ridiculously small numbers.
#define small_float(flt) (-FLOAT_THRESHOLD < (flt) && (flt) < FLOAT_THRESHOLD)

#endif