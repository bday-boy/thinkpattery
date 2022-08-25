#include <stddef.h>

#include "battery_tracker.h"
#include "files.h"

BatteryTracker * new_tracker() {
    BatteryTracker * tracker = malloc (sizeof(BatteryTracker));
    tracker->samples = SAMPLE_SIZE;
    tracker->mean_energy = 0.0;
    tracker->mean_time = 0.0;
    tracker->covariance = 0.0;
    tracker->time_variance = 0.0;
    tracker->index = 0;
    return tracker;
};

void del_tracker(BatteryTracker * tracker) {
    free(tracker);
};

void update_data(BatteryTracker * tracker) {
    // Update mean energy using simple moving average method
    double energy_new = bat0_energy_now() + bat1_energy_now();
    const double energy_old = tracker->energies[tracker->index];
    tracker->energies[tracker->index] = energy_new;
    tracker->mean_energy += (energy_new - energy_old) / tracker->samples;

    // Update index as circular array
    tracker->index = (tracker->index + 1) % tracker->samples;
};

void compute_regression(BatteryTracker * tracker) {
    tracker->current_time = system_uptime();
    tracker->mean_time = tracker->current_time - (tracker->samples / SAMPLE_FREQUENCY) / 2.0;
    compute_covariance(tracker);
    compute_time_variance(tracker);
};

// Important note: This technically calculates covariance * N. We omit dividing
// by N because it would cancel out when we divide covariance by variance
// anyway.
void compute_covariance(BatteryTracker * tracker) {
    tracker->covariance = 0.0;
    const double mean_energy = tracker->mean_energy;
    const double mean_time = tracker->mean_time;
    double time = tracker->current_time - SAMPLE_FREQUENCY * tracker->samples;
    for (int i = 0; i < tracker->samples; i++) {
        time += SAMPLE_FREQUENCY;
        double energy = tracker->energies[(tracker->index + i) % tracker->samples];
        tracker->covariance += (time - mean_time) * (energy - mean_energy);
    };
};

// Important note: This technically calculates variance * N. We omit dividing
// by N because it would cancel out when we divide covariance by variance
// anyway.
void compute_time_variance(BatteryTracker * tracker) {
    // TODO: Just use formula for variance of a uniform distribution
    tracker->time_variance = 0.0;
    const double mean_time = tracker->mean_time;
    double time = tracker->current_time - SAMPLE_FREQUENCY * tracker->samples;
    for (size_t i = 0; i < tracker->samples; i++) {
        time += SAMPLE_FREQUENCY;
        tracker->time_variance += (time - mean_time) * (time - mean_time);
    };
};

double find_remaining_time(BatteryTracker * tracker, int y_val) {
    get_current_time(tracker);
    compute_covariance(tracker);
    compute_time_variance(tracker);
    const double slope = tracker->covariance / tracker->time_variance;
    const double intercept = tracker->mean_energy - slope * tracker->mean_time;
    const double end_time = (y_val - intercept) / slope;
    const double remaining_time = end_time - tracker->current_time;
    return remaining_time;
};
