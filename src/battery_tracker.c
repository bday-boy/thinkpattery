#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "battery_tracker.h"
#include "files.h"

BatteryTracker * new_tracker() {
    BatteryTracker * tracker = malloc(sizeof(BatteryTracker));

    tracker->energy_full = bat0_energy_full() + bat1_energy_full();
    tracker->energy_now = bat0_energy_now() + bat1_energy_now();
    tracker->is_charging = is_charging();
    tracker->mode = PERCENT_MODE;
    tracker->regressor = new_regressor();

    return tracker;
};

void del_tracker(BatteryTracker * tracker) {
    del_regressor(tracker->regressor);
    free(tracker);
};

void update_tracker(BatteryTracker * tracker) {
    // Update mean energy using simple moving average method
    tracker->energy_now = bat0_energy_now() + bat1_energy_now();
    double new_uptime = system_uptime();
    update_regressor(tracker->regressor, new_uptime, tracker->energy_now);
};

double battery_percent(BatteryTracker * tracker) {
    if (
        -FLOAT_THRESHOLD < tracker->energy_full
        && tracker->energy_full < FLOAT_THRESHOLD
    ) {
        return NO_BATTERY_INFO;
    }
    return 100.0 * (tracker->energy_now / tracker->energy_full);
};

double seconds_until_end(BatteryTracker * tracker) {
    // When laptop is charging, we want to know the remaining seconds until
    // full. When it's discharging, we want to know when it reaches 0.
    const double y_val = tracker->energy_full * tracker->is_charging;
    return find_remaining_seconds(tracker->regressor, y_val);
};

// Prints battery icon and percent remaining
void print_battery_percent(BatteryTracker * tracker, const char * icon) {
    const double remaining_percent = battery_percent(tracker);
    if (remaining_percent == NO_BATTERY_INFO) {
        printf("N/A %%\n");
        return;
    }
    printf("%s %.1lf%%\n", icon, remaining_percent);
};

// Prints a float of seconds as hours and minutes
void print_time_left(BatteryTracker * tracker) {
    const double time_left = seconds_until_end(tracker);
    if (time_left == TIME_UNAVAILABLE) {
        printf("N/A Time\n");
        return;
    }
    printf("%.0lfmin left\n", time_left / 60.0);
};

void print_info(BatteryTracker * tracker, const char * icon) {
    switch (tracker->mode) {
        case PERCENT_MODE:
            print_battery_percent(tracker, icon);
            break;
        case REMAINING_TIME_MODE:
            print_time_left(tracker);
            break;
        default:
            printf("N/A\n");
            break;
    };
};