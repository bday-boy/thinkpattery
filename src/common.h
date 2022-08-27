#ifndef COMMON_H_
#define COMMON_H_

// Battery icon indices
#define BATTERY_CHARGING 0
#define BATTERY_EMPTY 1
#define BATTERY_QUARTER 2
#define BATTERY_HALF 3
#define BATTERY_THREE_QUARTERS 4
#define BATTERY_FULL 5

// Doesn't need to be very low -- the energies shown in each BATX/energy
// file usually only change every few seconds or so anyway
#define SAMPLE_FREQUENCY 1.0 // float in seconds

// Threshold for avoiding division by ridiculously small numbers
#define FLOAT_THRESHOLD 0.001
#define small_float(flt) (-FLOAT_THRESHOLD < (flt) && (flt) < FLOAT_THRESHOLD)

#endif