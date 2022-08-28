#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "battery_tracker.h"

const char * icons[] = {"", "", "", "", "", ""};

BatteryTracker * new_tracker() {
    BatteryTracker * tracker = malloc(sizeof(BatteryTracker));

    tracker->bfmanager = new_battery_file_manager();

    tracker->energy_full = bat_energy_full(tracker->bfmanager);
    tracker->energy_now = bat_energy_now(tracker->bfmanager);
    tracker->is_charging = is_charging(tracker->bfmanager);
    tracker->mode = PERCENT_MODE;

    tracker->exp_moving_avg = new_exp_moving_average(system_uptime(),
                                                     tracker->energy_now);

    return tracker;
};

void del_tracker(BatteryTracker * tracker) {
    del_exp_moving_average(tracker->exp_moving_avg);
    free(tracker);
};

void update_tracker(BatteryTracker * tracker) {
    // Update mean energy using simple moving average method
    tracker->energy_now = bat_energy_now(tracker->bfmanager);
    tracker->is_charging = is_charging(tracker->bfmanager);
    double new_uptime = system_uptime();
    progress_avg(tracker->exp_moving_avg, new_uptime, tracker->energy_now);
};

// Called each program iteration since we need data regardless
void rotate_display_mode(BatteryTracker * tracker) {
    tracker->mode = (tracker->mode + 1) % TOTAL_MODES;
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
    double goal_amount = tracker->energy_now \
        - tracker->energy_full * tracker->is_charging;
    return time_remaining(tracker->exp_moving_avg, goal_amount);
};

// Prints battery icon and percent remaining
void print_battery_percent(BatteryTracker * tracker) {
    const double remaining_percent = battery_percent(tracker);
    if (remaining_percent == NO_BATTERY_INFO) {
        printf("N/A %%\n");
        return;
    }
    const char * icon = get_icon(tracker->is_charging, remaining_percent);
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

void print_info(BatteryTracker * tracker) {
    switch (tracker->mode) {
        case PERCENT_MODE:
            print_battery_percent(tracker);
            break;
        case REMAINING_TIME_MODE:
            print_time_left(tracker);
            break;
        default:
            printf("Unknown mode\n");
            break;
    };
};

const char * get_icon(short charging_state, double percent_left) {
    if (charging_state) {
        return icons[BATTERY_CHARGING];
    } else if (percent_left < 10.0) {
        return icons[BATTERY_EMPTY];
    } else if (percent_left < 35.0) {
        return icons[BATTERY_QUARTER];
    } else if (percent_left < 65.0) {
        return icons[BATTERY_HALF];
    } else if (percent_left < 90.0) {
        return icons[BATTERY_THREE_QUARTERS];
    } else {
        return icons[BATTERY_FULL];
    }
};