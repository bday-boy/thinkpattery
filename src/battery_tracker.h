#ifndef BATTERY_TRACKER_H_
#define BATTERY_TRACKER_H_

#include "files.h"          // For BatteryFileManager
#include "moving_average.h" // For ExponentialMovingAverage

typedef enum output_modes_t {
    PERCENT_MODE = 0,
    REMAINING_TIME_MODE,
    BATTERY_HEALTH_MODE,

    // Keep at end and keep enum compact so it always represents total number
    // of modes.
    TOTAL_MODES
} output_modes_t;

typedef enum battery_percent_icons_t {
    BATTERY_EMPTY = 0,
    BATTERY_QUARTER,
    BATTERY_HALF,
    BATTERY_THREE_QUARTERS,
    BATTERY_FULL,

    // Keep at end and keep enum compact so it always represents total number
    // of icons.
    TOTAL_ICONS
} battery_percent_icons_t;

typedef struct BatteryTracker {
    // Reads data from AC file BATX files in the /sys/class/power_supply dir
    BatteryFileManager * bfmanager;

    // Battery state variables
    double energy_full;
    double energy_now;
    double battery_health;
    double prev_state_value;
    short is_charging;

    // Info display variables
    double print_variable;
    const char * icon;
    const char * print_format;
    output_modes_t mode;
    output_modes_t prev_mode;

    // Used for predicting when battery will be dead/fully charged
    ExponentialMovingAverage * exp_moving_avg;
} BatteryTracker;

BatteryTracker * new_tracker();
void del_tracker(BatteryTracker * tracker);

// Called each program iteration since we need data regardless.
void update_tracker(BatteryTracker * tracker);

// Changes the display mode and refreshes the data that doesn't need to be
// changed very often.
void rotate_display_mode(BatteryTracker * tracker);

// Used for avoiding unnecessary printing when nothing has changed
unsigned short no_state_change(BatteryTracker * tracker);

// Divides total current energy by total energy when full and multiplies by
// 100.0.
double battery_percent(BatteryTracker * tracker);

// Divides total energy when full by maximum total energy by design.
double battery_health(BatteryTracker * tracker);

// Gets number of seconds until reaching a certain energy value (usually 0 or
// energy_full).
double seconds_until_end(BatteryTracker * tracker);

// Prints battery icon and percent remaining.
void load_battery_percent(BatteryTracker * tracker);

// Prints a float of seconds as hours and minutes.
void load_time_left(BatteryTracker * tracker);

// Prints battery icon and percent remaining.
void load_battery_health(BatteryTracker * tracker);

// Prints battery info based on current display mode.
void print_info(BatteryTracker * tracker);

// Helpers

// Gets the appropriate icon for the current battery state.
const char * get_percent_icon(short charging_state, double percent_left);

#endif // BATTERY_TRACKER_H_