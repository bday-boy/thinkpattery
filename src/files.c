#include <stdio.h>  // For fscanf()
#include <unistd.h> // for access()

#include "files.h"

double system_uptime() {
    FILE* uptime_file = fopen(UPTIME, "r");
    if (uptime_file == NULL) {
        return NO_UPTIME;
    }
    double uptime;
    fscanf(uptime_file, "%lf", &uptime);
    fclose(uptime_file);
    return uptime;
};

short is_charging() {
    FILE* ac_online_file = fopen(AC_ONLINE, "r");
    if (ac_online_file == NULL) {
        return NO_CHARGING_INFO;
    }
    short charging;
    fscanf(ac_online_file, "%hd", &charging);
    fclose(ac_online_file);
    return charging;
};

double read_bat_file(char * fname) {
    FILE* bat_file = fopen(fname, "r");
    if (bat_file == NULL) {
        return NO_BATTERY_INFO;
    }
    double bat_stat;
    fscanf(bat_file, "%lf", &bat_stat);
    fclose(bat_file);
    return bat_stat;
};

double bat0_energy_full() {
    return read_bat_file(BAT0_FULL);
};

double bat1_energy_full() {
    return read_bat_file(BAT1_FULL);
};

double bat0_energy_now() {
    return read_bat_file(BAT0_NOW);
};

double bat1_energy_now() {
    return read_bat_file(BAT1_NOW);
};
