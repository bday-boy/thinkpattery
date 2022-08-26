#ifndef BATTERY_TRACKER_H_
#define BATTERY_TRACKER_H_

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
void update_tracker(BatteryTracker * tracker);

double battery_percent(BatteryTracker * tracker);

double seconds_until_end(BatteryTracker * tracker);

#endif // BATTERY_TRACKER_H_