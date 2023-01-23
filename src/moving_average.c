#include <math.h>
#include <stdlib.h>

#include "moving_average.h"

ExponentialMovingAverage * new_exp_moving_average(double initial_time,
                                                  double initial_sample) {
    ExponentialMovingAverage * exp_moving_avg =
        malloc(sizeof(ExponentialMovingAverage));

    exp_moving_avg->alpha = ALPHA;
    exp_moving_avg->speed = 0.0;
    exp_moving_avg->time = initial_time;
    exp_moving_avg->sample = initial_sample;
    exp_moving_avg->num_samples = 0;

    return exp_moving_avg;
};

void del_exp_moving_average(ExponentialMovingAverage * exp_moving_avg) {
    free(exp_moving_avg);
};

void progress_avg(ExponentialMovingAverage * exp_moving_avg,
                  double new_time, double new_sample) {
    // Ignore when the new sample is the same as the last one
    if (exp_moving_avg->sample == new_sample) {
        return;
    }

    double run = new_time - exp_moving_avg->time;
    double rise = new_sample - exp_moving_avg->sample;
    double speed = rise / run;

    // Speed is in battery energy per second
    if (exp_moving_avg->num_samples < NUM_INITIAL_SAMPLES) {
        exp_moving_avg->speed += speed / NUM_INITIAL_SAMPLES;
        exp_moving_avg->num_samples++;
    } else {
        exp_moving_avg->speed = (exp_moving_avg->alpha * speed)
            + ((1 - exp_moving_avg->alpha) * exp_moving_avg->speed);
    }

    exp_moving_avg->time = new_time;
    exp_moving_avg->sample = new_sample;
};

double time_remaining(ExponentialMovingAverage * exp_moving_avg,
                      double goal_amount) {
    // Don't wanna cause any massive blowup
    if (small_float(exp_moving_avg->speed)) {
        return TIME_UNAVAILABLE;
    }

    return fabs((goal_amount - exp_moving_avg->sample) / exp_moving_avg->speed);
};

unsigned short small_float(double flt) {
    return (-0.0001 < flt) && (flt < 0.0001);
};