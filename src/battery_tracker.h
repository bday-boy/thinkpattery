#ifndef BATTERY_TRACKER_H_
#define BATTERY_TRACKER_H_

#include "battery_info.h"   // For BatteryInfo
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
    // Manages battery state info, i.e. energy and charging status
    BatteryInfo * bat_info;

    // Info display variables
    double print_number;
    const char * icon;
    const char * print_format;
    output_modes_t mode;
    unsigned short changed_mode;
} BatteryTracker;

BatteryTracker * new_tracker();
void del_tracker(BatteryTracker * tracker);

// Changes the display mode and refreshes the data that doesn't need to be
// changed very often.
void rotate_display_mode(BatteryTracker * tracker);

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