#ifndef REGRESSOR_H_
#define REGRESSOR_H_

#include <stddef.h>

#include "common.h"

// TODO: Save times as an array and only update regressor when the battery
// changes energy

typedef struct TimeRegressor {
    size_t size;
    size_t length;

    // Sample values are the y-axis
    double samples[SAMPLE_SIZE];

    // Time values are the x-axis
    double times[SAMPLE_SIZE];

    // Statistical variables
    double mean_samples;
    double mean_time;

    // Represents the next index to get updated
    size_t index;
} TimeRegressor;

TimeRegressor * new_regressor();
void del_regressor(TimeRegressor * regressor);

void update_regressor(TimeRegressor * regressor, double new_uptime, double new_sample);

// Only called when the program wants to predict remaining progress
double compute_covariance(TimeRegressor * regressor);
double compute_time_variance(TimeRegressor * regressor);
double find_remaining_seconds(TimeRegressor * regressor, double y_val);

// Helpers
size_t get_prev_index(TimeRegressor * regressor);

#endif // REGRESSOR_H_