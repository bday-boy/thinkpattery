#ifndef COMMON_H_
#define COMMON_H_

// Number of battery energies we want to keep track of. A higher number will
// make the tracker take a longer amount of time to have meaningful data
#define SAMPLE_SIZE 32

// Doesn't need to be very low -- the energies shown in each BATX/energy
// file usually only change every few seconds or so anyway
#define SAMPLE_FREQUENCY 0.1 // float in seconds

// Threshold for avoiding division by ridiculously small numbers
#define FLOAT_THRESHOLD 0.001

#endif