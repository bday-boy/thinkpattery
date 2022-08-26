#include <stdio.h>  // For fscanf()
#include <unistd.h> // for access()

#include "files.h"

double system_uptime() {
    double uptime;
    FILE* uptime_file = fopen(UPTIME, "r");
    fscanf(uptime_file, "%lf", &uptime);
    return uptime;
};

short is_charging() {
    short charging;
    FILE* ac_online_file = fopen(AC_ONLINE, "r");
    fscanf(ac_online_file, "%hd", &charging);
    return charging;
};

double read_bat_file(char * fname) {
    if (access(fname, F_OK) != 0) {
        return 0.0;
    }
    double bat_stat;
    FILE* bat_file = fopen(fname, "r");
    fscanf(bat_file, "%lf", &bat_stat);
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
