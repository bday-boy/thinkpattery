#include "time_regression.h"
#include "files.h"

TimeRegressor * new_regressor(double sample_interval) {
    // This function assumes the memory for the samples array and index_ptr
    // are already allocated
    TimeRegressor * regressor = malloc (sizeof(TimeRegressor));

    regressor->num_samples = 0;
    regressor->samples_size = SAMPLE_SIZE;
    regressor->sample_interval = sample_interval;

    regressor->mean_samples = 0.0;
    regressor->mean_time = 0.0;

    regressor->index = 0;
};

void del_regressor(TimeRegressor * regressor) {
    free(regressor);
};

void update_regressor(TimeRegressor * regressor, double new_sample) {
    // Use Simple Moving Average method to update the average
    const double old_sample = regressor->samples[regressor->index];
    regressor->samples[regressor->index] = new_sample;
    regressor->mean_samples += (new_sample - old_sample) / regressor->samples_size;

    // Update index as circular array
    regressor->index = (regressor->index + 1) % regressor->samples_size;
    regressor->num_samples++;
};

void set_regression_variables(TimeRegressor * regressor) {
    regressor->current_time = system_uptime();
    regressor->start_time = regressor->current_time - regressor->sample_interval * regressor->samples_size;
};

double compute_covariance(TimeRegressor * regressor) {
    double covariance = 0.0;
    double time = regressor->start_time;
    for (size_t i = 0; i < regressor->num_samples; i++) {
        time += regressor->sample_interval;
        const size_t sample_index = (regressor->index + i) % regressor->samples_size;
        const double sample_value = regressor->samples[sample_index];
        covariance += (time - regressor->mean_time) * (sample_value - regressor->mean_samples);
    };
    return covariance;
};

double compute_time_variance(TimeRegressor * regressor) {
    double time_variance = 0.0;
    double time = regressor->start_time;
    for (size_t i = 0; i < regressor->num_samples; i++) {
        time += regressor->sample_interval;
        time_variance += (time - regressor->mean_time) * (time - regressor->mean_time);
    };
    return time_variance;
};

double find_remaining_seconds(TimeRegressor * regressor, double y_val) {
    set_regression_variables(regressor);
    const double covariance = compute_covariance(regressor);
    const double time_variance = compute_time_variance(regressor);

    // Get m and b for y = mx + b
    const double slope = covariance / time_variance;
    const double intercept = regressor->mean_samples - slope * regressor->mean_time;

    // Solve y = mx + b for x when y = y_val (either 0 or full battery)
    const double end_time = (y_val - intercept) / slope;
    const double remaining_seconds = end_time - regressor->current_time;

    return remaining_seconds;
};
