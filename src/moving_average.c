#include <stdlib.h>

#include "common.h"
#include "moving_average.h"

ExponentialMovingAverage * new_exp_moving_average(double initial_time,
                                                  double initial_sample) {
    ExponentialMovingAverage * exp_moving_avg = \
        malloc(sizeof(ExponentialMovingAverage));

    exp_moving_avg->alpha = ALPHA;
    exp_moving_avg->speed = 0.0;
    exp_moving_avg->time = initial_time;
    exp_moving_avg->sample = initial_sample;

    return exp_moving_avg;
};

void del_exp_moving_average(ExponentialMovingAverage * exp_moving_avg) {
    free(exp_moving_avg);
};

double progress_avg(ExponentialMovingAverage * exp_moving_avg,
                    double new_time, double new_sample) {
    const double run = new_time - exp_moving_avg->time;
    const double rise = new_sample - exp_moving_avg->sample;
    const double speed = rise / run;

    if (small_float(speed)) {
        return VALUE_UNAVAILABLE;
    }

    // Speed is in battery energy per second
    exp_moving_avg->speed = (exp_moving_avg->alpha * speed) \
        + ((1 - exp_moving_avg->alpha) * exp_moving_avg->speed);
    exp_moving_avg->time = new_time;
    exp_moving_avg->sample = new_sample;

    return new_sample / exp_moving_avg->speed;
};