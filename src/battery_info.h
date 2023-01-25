#ifndef BATTERY_INFO_H_
#define BATTERY_INFO_H_

#include "files.h" // For BatteryFileManager
#include "moving_average.h"

typedef struct BatteryInfo {
    // Reads data from AC file, BATX files in the /sys/class/power_supply dir
    BatteryFileManager * bfmanager;

    // Keeps track of the rate at which the battery is changing energy
    ExponentialMovingAverage * moving_avg;

    // Battery state variables read from files
    double energy_now;
    double energy_full;
    double energy_design;
    short is_charging;

    // Battery state variables computed from the above variables
    double percent_now;
    double battery_health;

    // Useful to avoid unnecessary work
    unsigned short state_changed;
} BatteryInfo;

BatteryInfo * new_bat_info();
void init_bat_info(BatteryInfo * bat_info);
void del_battery_info(BatteryInfo * bat_info);

void * update_energy(BatteryInfo * bat_info);
void * update_health(BatteryInfo * bat_info);

#endif // BATTERY_INFO_H_