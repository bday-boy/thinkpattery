#ifndef REGRESSOR_H_
#define REGRESSOR_H_

#include <stddef.h>

#include "common.h"

// TODO: Save times as an array and only update regressor when the battery
// changes energy

typedef struct TimeRegressor {
    // Samples represent y-values
    double samples[SAMPLE_SIZE];
    size_t samples_size;
    size_t num_samples;

    // Used to generate the time variance
    double start_time;
    double current_time;
    double sample_interval;

    // Statistical variables
    double mean_samples;
    double mean_time;

    // Represents the next index to get updated
    size_t index;
} TimeRegressor;

TimeRegressor * new_regressor();
void del_regressor(TimeRegressor * regressor);

void update_regressor(TimeRegressor * regressor, double new_sample);

// Only called when the program wants to predict remaining progress
void set_regression_variables(TimeRegressor * regressor);
double compute_covariance(TimeRegressor * regressor);
double compute_time_variance(TimeRegressor * regressor);
double find_remaining_seconds(TimeRegressor * regressor, double y_val);

#endif // REGRESSOR_H_