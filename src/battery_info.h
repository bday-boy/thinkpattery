#ifndef BATTERY_INFO_H_
#define BATTERY_INFO_H_

#include "files.h" // For BatteryFileManager

typedef struct BatteryInfo {
    // Reads data from AC file, BATX files in the /sys/class/power_supply dir
    BatteryFileManager * bfmanager;

    // Battery state variables read from files
    double energy_now;
    double energy_full;
    double energy_design;
    short is_charging;

    // Battery state variables computed from the above variables
    double percent_now;
    double battery_health;

    // Useful to avoid unnecessary work in the battery tracker
    unsigned short state_changed;
} BatteryInfo;

BatteryInfo * new_bat_info();
void del_battery_info(BatteryInfo * bat_info);

void update_info(BatteryInfo * bat_info);
void update_percent(BatteryInfo * bat_info);
void update_health(BatteryInfo * bat_info);

double get_bat_health(BatteryInfo * bat_info);

#endif // BATTERY_INFO_H_