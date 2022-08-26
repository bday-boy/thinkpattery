#ifndef BATTERY_TRACKER_H_
#define BATTERY_TRACKER_H_

// Number of battery energies we want to keep track of. A higher number will
// make the tracker take a longer amount of time to have meaningful data
#define SAMPLE_SIZE 32

// Doesn't need to be very low -- the energies shown in each BATX/energy
// file usually only change every few seconds or so anyway
#define SAMPLE_FREQUENCY 1.0 // float in seconds

#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "time_regression.h"

typedef struct BatteryTracker {
    // Battery state variables
    double energy_now;
    double energy_full;
    short is_charging;
    
    // Regressor used for predicting when battery will be dead/fully charged
    TimeRegressor * regressor;
} BatteryTracker;

BatteryTracker * new_tracker();
void del_tracker(BatteryTracker * tracker);

// Called each program iteration since we need data regardless
void update_data(BatteryTracker * tracker);

double battery_percent(BatteryTracker * tracker);

double seconds_until_end(BatteryTracker * tracker);

#endif // BATTERY_TRACKER_H_