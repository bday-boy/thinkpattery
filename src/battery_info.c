#include <stdlib.h> // For malloc(), free()

#include "battery_info.h"

BatteryInfo * new_bat_info() {
    BatteryInfo * bat_info = malloc(sizeof(BatteryInfo));

    bat_info->bfmanager = new_battery_file_manager();

    bat_info->energy_full = bat_energy_full(bat_info->bfmanager);
    bat_info->energy_now = bat_energy_now(bat_info->bfmanager);
    bat_info->energy_design = bat_energy_design(bat_info->bfmanager);
    bat_info->is_charging = bat_is_charging(bat_info->bfmanager);

    update_percent(bat_info);
    update_health(bat_info);

    bat_info->state_changed = 0;

    return bat_info;
};

void del_battery_info(BatteryInfo * bat_info) {
    del_battery_file_manager(bat_info->bfmanager);
    free(bat_info);
};

void update_info(BatteryInfo * bat_info) {
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
        update_percent(bat_info);
    }
};

void update_percent(BatteryInfo * bat_info) {
    if (bat_info->energy_now > 0.0 && bat_info->energy_full > 0.0) {
        bat_info->percent_now = 100.0
            * (bat_info->energy_now / bat_info->energy_full);
    } else {
        bat_info->percent_now = NO_INFO;
    }
};

void update_health(BatteryInfo * bat_info) {
    bat_info->energy_full = bat_energy_full(bat_info->bfmanager);
    bat_info->energy_design = bat_energy_design(bat_info->bfmanager);

    if (bat_info->energy_full > 0.0 && bat_info->energy_design > 0.0) {
        bat_info->battery_health = 100.0
            * (bat_info->energy_full / bat_info->energy_design);
    } else {
        bat_info->battery_health = NO_INFO;
    }
};