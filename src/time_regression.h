#ifndef REGRESSOR_H_
#define REGRESSOR_H_

// Number of battery energies we want to keep track of. A higher number will
// make the tracker take a longer amount of time to have meaningful data
#define SAMPLE_SIZE 32

#include <stddef.h>

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

TimeRegressor * new_regressor(double sample_interval);
void del_regressor(TimeRegressor * regressor);

void update_regressor(TimeRegressor * regressor, double new_sample);

// Only called when the program wants to predict remaining progress
void set_regression_variables(TimeRegressor * regressor);
double compute_covariance(TimeRegressor * regressor);
double compute_time_variance(TimeRegressor * regressor);
double find_remaining_seconds(TimeRegressor * regressor, double y_val);

#endif // REGRESSOR_H_