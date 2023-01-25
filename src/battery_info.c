#include <stdlib.h> // For malloc(), free()

#include "battery_info.h"

// Given a part and a whole, find what percent of the whole is taken up by the
// part and set its value into the percent_addr address. Set to -1 if either
// number is negative.
void calculate_percent(double part, double whole, double * percent_addr) {
    if (part >= 0.0 && whole > 0.0) {
        *percent_addr = 100.0 * (part / whole);
    } else {
        *percent_addr = NO_INFO;
    }
};

BatteryInfo * new_bat_info() {
    BatteryInfo * bat_info = malloc(sizeof(BatteryInfo));
    init_bat_info(bat_info);
    return bat_info;
};

void init_bat_info(BatteryInfo * bat_info) {
    bat_info->bfmanager = new_battery_file_manager();

    bat_info->energy_full = bat_energy_full(bat_info->bfmanager);
    bat_info->energy_now = bat_energy_now(bat_info->bfmanager);
    bat_info->energy_design = bat_energy_design(bat_info->bfmanager);
    bat_info->is_charging = bat_is_charging(bat_info->bfmanager);

    bat_info->moving_avg = new_exp_moving_average(bat_info->energy_now);

    calculate_percent(bat_info->energy_now, bat_info->energy_full,
                        &bat_info->percent_now);
    calculate_percent(bat_info->energy_full, bat_info->energy_design,
                      &bat_info->battery_health);

    bat_info->state_changed = 1;
};

void del_battery_info(BatteryInfo * bat_info) {
    del_battery_file_manager(bat_info->bfmanager);
    del_exp_moving_average(bat_info->moving_avg);
    free(bat_info);
};

void * update_energy(BatteryInfo * bat_info) {
    // Read new data from battery and AC files
    double energy_now = bat_energy_now(bat_info->bfmanager);
    short is_charging = bat_is_charging(bat_info->bfmanager);

    // State has changed when battery energy or charging state is different
    // from the previous value
    bat_info->state_changed = (
        energy_now != bat_info->energy_now
        || is_charging != bat_info->is_charging
    );

    if (bat_info->state_changed) {
        bat_info->energy_now = energy_now;
        bat_info->is_charging = is_charging;
        calculate_percent(energy_now, bat_info->energy_full,
                          &bat_info->percent_now);
        progress_avg(bat_info->moving_avg, energy_now);
    }

    return NULL;
};

void * update_health(BatteryInfo * bat_info) {
    // We are defining "health" as the ratio of the battery's current maximum
    // energy capacity and the battery's factory maximum energy capacity
    bat_info->energy_full = bat_energy_full(bat_info->bfmanager);
    bat_info->energy_design = bat_energy_design(bat_info->bfmanager);
    calculate_percent(bat_info->energy_full, bat_info->energy_design,
                      &bat_info->battery_health);
    return NULL;
};