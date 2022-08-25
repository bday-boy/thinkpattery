#include <stdio.h>  // For fscanf
#include <stdint.h> // For int32_t

#include "files.h"

double system_uptime() {
    double uptime;
    FILE* uptime_file = fopen(UPTIME, "r");
    fscanf(uptime_file, "%lf", &uptime);
    return uptime;
};

uint8_t is_charging() {
    short charging;
    FILE* ac_online_file = fopen(AC_ONLINE, "r");
    fscanf(ac_online_file, "%d", &charging);
    return charging;
};

int32_t read_bat_file(char * filename) {
    int32_t bat_stat;
    FILE* bat_file = fopen(filename, "r");
    fscanf(bat_file, "%d", &bat_stat);
    return bat_stat;
};

int32_t bat0_energy_full() {
    return read_bat_file(BAT0_FULL);
};

int32_t bat1_energy_full() {
    return read_bat_file(BAT1_FULL);
};

int32_t bat0_energy_now() {
    return read_bat_file(BAT0_NOW);
};

int32_t bat1_energy_now() {
    return read_bat_file(BAT1_NOW);
};
