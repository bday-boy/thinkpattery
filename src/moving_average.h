#ifndef MOVING_AVERAGE_H_
#define MOVING_AVERAGE_H_

#define ALPHA 0.05
#define TIME_UNAVAILABLE -1.0

// A time-based exponential moving average calculator
typedef struct ExponentialMovingAverage {
    // Smoothing factor - higher prioritizes most recent slope
    double alpha;

    // Current "speed"
    double speed;

    // Current x value
    double time;

    // Current y value
    double sample;
} ExponentialMovingAverage;

ExponentialMovingAverage * new_exp_moving_average(double initial_time,
                                                  double initial_sample);
void del_exp_moving_average(ExponentialMovingAverage * exp_moving_avg);

void progress_avg(ExponentialMovingAverage * exp_moving_avg,
                  double new_time, double new_sample);

double time_remaining(ExponentialMovingAverage * exp_moving_avg,
                      double goal_amount);

#endif // MOVING_AVERAGE_H_