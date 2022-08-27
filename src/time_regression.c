#include <stdlib.h>

#include "time_regression.h"
#include "files.h"

TimeRegressor * new_regressor() {
    TimeRegressor * regressor = malloc(sizeof(TimeRegressor));

    regressor->length = 0;
    regressor->size = SAMPLE_SIZE;
    regressor->mean_samples = 0.0;
    regressor->mean_time = 0.0;
    regressor->index = 0;

    return regressor;
};

void del_regressor(TimeRegressor * regressor) {
    free(regressor);
};

void update_regressor(TimeRegressor * regressor, double new_uptime, double new_sample) {
    // Only update when the new sample is different
    const double prev_sample = regressor->samples[get_prev_index(regressor)];
    if (prev_sample == new_sample) {
        return;
    }

    // Use Simple Moving Average method to update the averages
    const double old_sample = regressor->samples[regressor->index];
    regressor->samples[regressor->index] = new_sample;
    regressor->mean_samples += (new_sample - old_sample) / regressor->size;

    const double old_uptime = regressor->times[regressor->index];
    regressor->samples[regressor->index] = new_uptime;
    regressor->mean_time += (new_uptime - old_uptime) / regressor->size;

    // Update index as circular array
    regressor->index = (regressor->index + 1) % regressor->size;
    if (regressor->length < regressor->size) {
        regressor->length++;
    }
};

double compute_covariance(TimeRegressor * regressor) {
    double covariance = 0.0;
    for (size_t i = 0; i < regressor->length; i++) {
        const size_t index = (regressor->index + i) % regressor->size;
        const double sample_value = regressor->samples[index];
        const double time_value = regressor->times[index];
        covariance += (time_value - regressor->mean_time) * (sample_value - regressor->mean_samples);
    };
    return covariance;
};

double compute_time_variance(TimeRegressor * regressor) {
    double time_variance = 0.0;
    for (size_t i = 0; i < regressor->length; i++) {
        const size_t index = (regressor->index + i) % regressor->size;
        const double time = regressor->samples[index];
        time_variance += (time - regressor->mean_time) * (time - regressor->mean_time);
    };
    return time_variance;
};

double find_remaining_seconds(TimeRegressor * regressor, double y_val) {
    const double covariance = compute_covariance(regressor);
    const double time_variance = compute_time_variance(regressor);

    // Avoid dividing by zero
    if (-FLOAT_THRESHOLD < time_variance && time_variance < FLOAT_THRESHOLD) {
        return -1.0;
    }

    // Get m and b for y = mx + b
    const double slope = covariance / time_variance;
    const double intercept = regressor->mean_samples - slope * regressor->mean_time;

    // Avoid dividing by zero
    if (-FLOAT_THRESHOLD < slope && slope < FLOAT_THRESHOLD) {
        return -1.0;
    }

    // Solve y = mx + b for x when y = y_val (either 0 or full battery)
    const double end_time = (y_val - intercept) / slope;

    // Get current time from prev array value
    const double remaining_seconds = end_time - regressor->times[get_prev_index(regressor)];

    return remaining_seconds;
};

size_t get_prev_index(TimeRegressor * regressor) {
    // Can't just do (index - 1) % 1 since it's an unsigned long int
    if (regressor->index == 0) {
        return regressor->size - 1;
    } else {
        return regressor->index - 1;
    }
};