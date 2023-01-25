#ifndef MOVING_AVERAGE_H_
#define MOVING_AVERAGE_H_

#include <stddef.h>

#define ALPHA 0.1
#define NUM_INITIAL_SAMPLES 5
#define TIME_UNAVAILABLE -1.0

// A time-based exponential moving average (EMA) calculator.
typedef struct ExponentialMovingAverage {
    // Smoothing factor - higher prioritizes most recent slope.
    double alpha;

    // Current slope of the line.
    double speed;

    // Time value from the most recent update.
    double time;

    // Sample value from the most recent update.
    double sample;

    // Used to track the first couple of samples.
    size_t num_samples;
} ExponentialMovingAverage;

ExponentialMovingAverage * new_exp_moving_average(double initial_sample);
void del_exp_moving_average(ExponentialMovingAverage * exp_moving_avg);

void progress_avg(ExponentialMovingAverage * exp_moving_avg, double new_sample);

double time_remaining(ExponentialMovingAverage * exp_moving_avg,
                      double goal_amount);

unsigned short small_float(double flt);

#endif // MOVING_AVERAGE_H_