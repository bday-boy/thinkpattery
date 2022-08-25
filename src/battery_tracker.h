#ifndef BATTERY_TRACKER_H_
#define BATTERY_TRACKER_H_

// Number of battery energies we want to keep track of. A higher number will
// make the tracker take a longer amount of time to have meaningful data
#define SAMPLE_SIZE 32

// Doesn't need to be very low -- the energies shown in each BATX/energy
// file usually only change every few seconds or so anyway
#define SAMPLE_FREQUENCY 2.0 // float in seconds

#include <stddef.h>
#include <stdint.h>

typedef struct BatteryTracker {
    // Regression variables
    int16_t samples;
    double energies[SAMPLE_SIZE];
    double energy_now;
    double energy_full;
    double current_time;

    // Statistical variables
    double mean_energy;
    double mean_time;
    double covariance;
    double time_variance;

    // Represents the next index to get updated
    size_t index;
} BatteryTracker;

BatteryTracker * new_tracker();
void del_tracker(BatteryTracker * tracker);

// Called each program iteration since we need data regardless
void update_data(BatteryTracker * tracker);

// Only called when program is in tracker mode
void compute_regression(BatteryTracker * tracker);

// Only called when the program wants to predict remaining progress
void compute_covariance(BatteryTracker * tracker);
void compute_time_variance(BatteryTracker * tracker);
double find_remaining_time(BatteryTracker * tracker, int y_val);

#endif // BATTERY_TRACKER_H_