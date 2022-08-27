#ifndef BATTERY_TRACKER_H_
#define BATTERY_TRACKER_H_

#include <stddef.h>

#include "common.h"
#include "moving_average.h"

#define PERCENT_MODE 0
#define REMAINING_TIME_MODE 1

extern const char * icons[];

typedef struct BatteryTracker {
    // Battery state variables
    double energy_now;
    double energy_full;
    short is_charging;
    unsigned short mode;

    // Used for predicting when battery will be dead/fully charged
    ExponentialMovingAverage * exp_moving_avg;
} BatteryTracker;

BatteryTracker * new_tracker();
void del_tracker(BatteryTracker * tracker);

// Called each program iteration since we need data regardless
void update_tracker(BatteryTracker * tracker);

// Gets remaining battery percent
double battery_percent(BatteryTracker * tracker);

// Gets number of seconds until reaching a certain energy value
double seconds_until_end(BatteryTracker * tracker);

// Prints battery icon and percent remaining
void print_battery_percent(BatteryTracker * tracker);

// Prints a float of seconds as hours and minutes
void print_time_left(BatteryTracker * tracker);

// Prints battery info based on modec
void print_info(BatteryTracker * tracker);

// Helpers

// Gets the appropriate icon for the current battery state
const char * get_icon(short charging_state, double percent_left);

#endif // BATTERY_TRACKER_H_