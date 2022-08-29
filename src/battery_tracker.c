#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "battery_tracker.h"
#include "common.h"

const char * charging_icon = "";
const char * percent_icons[] = {"", "", "", "", ""};
const char * time_icon = "";
const char * health_icon_good = "";
const char * health_icon_bad = "";

const char * percent_format = "%s %.1lf%%\n";
const char * time_format = "%s %.0lfmin\n";
const char * health_format = "%s %.1lf%%\n";

BatteryTracker * new_tracker() {
    BatteryTracker * tracker = malloc(sizeof(BatteryTracker));

    tracker->bfmanager = new_battery_file_manager();

    tracker->energy_full = bat_energy_full(tracker->bfmanager);
    tracker->energy_now = bat_energy_now(tracker->bfmanager);
    tracker->battery_health = 100.0 * tracker->energy_full \
        / bat_energy_design(tracker->bfmanager);
    tracker->is_charging = is_charging(tracker->bfmanager);
    tracker->prev_state_value = -1.0;

    tracker->icon = percent_icons[0];
    tracker->print_variable = -1.0;
    tracker->print_format = percent_format;
    tracker->mode = PERCENT_MODE;
    tracker->prev_mode = PERCENT_MODE;

    tracker->exp_moving_avg = new_exp_moving_average(system_uptime(),
                                                     tracker->energy_now);

    return tracker;
};

void del_tracker(BatteryTracker * tracker) {
    del_battery_file_manager(tracker->bfmanager);
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

void rotate_display_mode(BatteryTracker * tracker) {
    // First, update health, since it doesn't need to be updated each program
    // loop but should be updated before it displays
    tracker->energy_full = bat_energy_full(tracker->bfmanager);
    tracker->battery_health = 100.0 * tracker->energy_full \
        / bat_energy_design(tracker->bfmanager);

    tracker->mode = (tracker->mode + 1) % TOTAL_MODES;
};

double no_state_change(BatteryTracker * tracker, double new_state_value) {
    // We should indicate no state change only if the display mode is the same
    // and the prev_state_value is very close to the new_state_value
    return (tracker->mode == tracker->prev_mode) \
        && (small_float(tracker->prev_state_value - new_state_value));
};

double battery_percent(BatteryTracker * tracker) {
    if (
        -FLOAT_THRESHOLD < tracker->energy_full
        && tracker->energy_full < FLOAT_THRESHOLD
    ) {
        return NO_ENERGY_INFO;
    }
    return 100.0 * (tracker->energy_now / tracker->energy_full);
};

double seconds_until_end(BatteryTracker * tracker) {
    // When laptop is charging, we want to know the remaining seconds until
    // full. When it's discharging, we want to know when it reaches 0.
    double goal_amount = tracker->energy_full * tracker->is_charging;
    return time_remaining(tracker->exp_moving_avg, goal_amount);
};

void load_battery_percent(BatteryTracker * tracker) {
    tracker->print_format = percent_format;
    const double remaining_percent = battery_percent(tracker);
    if (remaining_percent == NO_ENERGY_INFO) {
        tracker->print_variable = remaining_percent;
        tracker->icon = percent_icons[0];
        return;
    }
    tracker->icon = get_percent_icon(tracker->is_charging, remaining_percent);
    tracker->print_variable = remaining_percent;
};

void load_time_left(BatteryTracker * tracker) {
    tracker->print_format = time_format;
    const double time_left = seconds_until_end(tracker);
    if (time_left == TIME_UNAVAILABLE) {
        tracker->print_variable = time_left;
        tracker->icon = percent_icons[0];
        return;
    }
    tracker->icon = time_icon;
    tracker->print_variable = time_left / 60.0;
};

void load_battery_health(BatteryTracker * tracker) {
    tracker->print_format = health_format;
    if (tracker->battery_health <= FLOAT_THRESHOLD) {
        tracker->print_variable = NO_HEALTH_INFO;
        tracker->icon = percent_icons[0];
        return;
    }
    tracker->icon = (tracker->battery_health > 60.0)
        ? health_icon_good
        : health_icon_bad;
    tracker->print_variable = tracker->battery_health;
};

void print_info(BatteryTracker * tracker) {
    tracker->prev_mode = tracker->mode;
    switch (tracker->mode) {
        case PERCENT_MODE:
            load_battery_percent(tracker);
            break;
        case REMAINING_TIME_MODE:
            load_time_left(tracker);
            break;
        case BATTERY_HEALTH_MODE:
            load_battery_health(tracker);
            break;
        default:
            printf("Unknown mode\n");
            return;
    };
    if (no_state_change(tracker, tracker->print_variable)) {
        return;
    }
    printf(tracker->print_format, tracker->icon, tracker->print_variable);
    tracker->prev_state_value = tracker->print_variable;
};

const char * get_percent_icon(short charging_state, double percent_left) {
    if (charging_state) {
        return charging_icon;
    } else if (percent_left < 10.0) {
        return percent_icons[BATTERY_EMPTY];
    } else if (percent_left < 35.0) {
        return percent_icons[BATTERY_QUARTER];
    } else if (percent_left < 65.0) {
        return percent_icons[BATTERY_HALF];
    } else if (percent_left < 90.0) {
        return percent_icons[BATTERY_THREE_QUARTERS];
    } else {
        return percent_icons[BATTERY_FULL];
    }
};