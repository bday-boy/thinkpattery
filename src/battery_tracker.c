#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "battery_tracker.h"

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

    tracker->bat_info = new_bat_info();

    tracker->icon = percent_icons[0];
    tracker->print_number = -1.0;
    tracker->print_format = percent_format;
    tracker->mode = PERCENT_MODE;
    tracker->changed_mode = 0;

    return tracker;
};

void del_tracker(BatteryTracker * tracker) {
    del_battery_info(tracker->bat_info);
    free(tracker);
};

void rotate_display_mode(BatteryTracker * tracker) {
    tracker->mode = (tracker->mode + 1) % TOTAL_MODES;
    switch (tracker->mode) {
        case PERCENT_MODE:
            tracker->print_format = percent_format;
            break;
        case REMAINING_TIME_MODE:
            tracker->print_format = time_format;
            break;
        case BATTERY_HEALTH_MODE:
            tracker->print_format = health_format;
            update_health(tracker->bat_info);
            break;
        default:
            exit(EXIT_FAILURE);
    };
    tracker->changed_mode = 1;
};

double seconds_until_end(BatteryTracker * tracker) {
    // When laptop is charging, we want to know the remaining seconds until
    // full. When it's discharging, we want to know when it reaches 0.
    double goal_amount = tracker->bat_info->energy_full
        * tracker->bat_info->is_charging;
    return time_remaining(tracker->bat_info->moving_avg, goal_amount);
};

void load_battery_percent(BatteryTracker * tracker) {
    const double remaining_percent = tracker->bat_info->percent_now;
    if (remaining_percent < 0.0) {
        tracker->print_number = remaining_percent;
        tracker->icon = percent_icons[0];
        return;
    }
    tracker->icon = get_percent_icon(tracker->bat_info->is_charging,
                                     remaining_percent);
    tracker->print_number = remaining_percent;
};

void load_time_left(BatteryTracker * tracker) {
    const double time_left = seconds_until_end(tracker);
    if (time_left == TIME_UNAVAILABLE) {
        tracker->print_number = time_left;
        tracker->icon = percent_icons[0];
        return;
    }
    tracker->icon = time_icon;
    tracker->print_number = time_left / 60.0;
};

void load_battery_health(BatteryTracker * tracker) {
    if (tracker->bat_info->battery_health < 0.0) {
        tracker->print_number = NO_INFO;
        tracker->icon = health_icon_bad;
        return;
    }
    tracker->icon = (tracker->bat_info->battery_health > 60.0)
        ? health_icon_good
        : health_icon_bad;
    tracker->print_number = tracker->bat_info->battery_health;
};

void print_info(BatteryTracker * tracker) {
    if (!tracker->bat_info->state_changed && !tracker->changed_mode) {
        return;
    }
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
            exit(EXIT_FAILURE);
    };
    printf(tracker->print_format, tracker->icon, tracker->print_number);
    tracker->changed_mode = 0;
};

const char * get_percent_icon(short is_charging, double percent_left) {
    if (is_charging) {
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