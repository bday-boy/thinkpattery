#include <dirent.h> // For struct dirent, DIR, readdir(), closedir()
#include <stdio.h>  // For fscanf()
#include <stdlib.h> // For malloc()

#include "common.h"
#include "files.h"
#include "utils.h"

const char * ac_dir = AC_DIR;
const char * bat_dir = BAT_DIR;
const char * ac_online_file = AC_ONLINE_FILE;
const char * energy_design = ENERGY_DESIGN;
const char * energy_full = ENERGY_FULL;
const char * energy_now = ENERGY_NOW;
const char * power_supply_dir = POWER_SUPPLY_DIR;

BatteryFileManager * new_battery_file_manager() {
    BatteryFileManager * bfmanager = malloc(sizeof(BatteryFileManager));
    bfmanager->num_batteries = 0;

    const size_t file_memsize = sizeof(char) * MAX_FILENAME_LEN;

    // Count number of directories in power_supply that start with BAT
    struct dirent * dp;
    DIR * dfd = opendir(power_supply_dir);
    if (dfd == NULL) {
        // TODO: Error log here, maybe even exit program
        return NULL;
    }
    while ((dp = readdir(dfd)) != NULL) {
        bfmanager->num_batteries += starts_with(dp->d_name, bat_dir, 3);
    }
    closedir(dfd);

    // Initialize all string arrays of relevant energy files
    const size_t num_battery_dirs = sizeof(char*) * bfmanager->num_batteries;
    bfmanager->energy_design_files = malloc(num_battery_dirs);
    bfmanager->energy_full_files = malloc(num_battery_dirs);
    bfmanager->energy_now_files = malloc(num_battery_dirs);
    dfd = opendir(power_supply_dir);
    size_t index = 0;
    while ((dp = readdir(dfd)) != NULL) {
        if (!starts_with(dp->d_name, bat_dir, 3)) {
            continue;
        }
        bfmanager->energy_design_files[index] = malloc(file_memsize);
        bfmanager->energy_full_files[index] = malloc(file_memsize);
        bfmanager->energy_now_files[index] = malloc(file_memsize);
        snprintf(bfmanager->energy_design_files[index], MAX_FILENAME_LEN,
                 "%s/%s/%s", power_supply_dir, dp->d_name, energy_design);
        snprintf(bfmanager->energy_full_files[index], MAX_FILENAME_LEN,
                 "%s/%s/%s", power_supply_dir, dp->d_name, energy_full);
        snprintf(bfmanager->energy_now_files[index], MAX_FILENAME_LEN,
                 "%s/%s/%s", power_supply_dir, dp->d_name, energy_now);
        index++;
    }
    closedir(dfd);

    bfmanager->ac_online_file = malloc(file_memsize);
    snprintf(bfmanager->ac_online_file, MAX_FILENAME_LEN, "%s/%s/%s",
             power_supply_dir, ac_dir, ac_online_file);

    return bfmanager;
};

void del_battery_file_manager(BatteryFileManager * bfmanager) {
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        free(bfmanager->energy_design_files[index]);
        free(bfmanager->energy_full_files[index]);
        free(bfmanager->energy_now_files[index]);
    }
    free(bfmanager->energy_design_files);
    free(bfmanager->energy_full_files);
    free(bfmanager->energy_now_files);
    free(bfmanager->ac_online_file);
    free(bfmanager);
};

short is_charging(BatteryFileManager * bfmanager) {
    FILE* ac_online_file = fopen(bfmanager->ac_online_file, "r");
    if (ac_online_file == NULL) {
        return NO_CHARGING_INFO;
    }
    short charging;
    fscanf(ac_online_file, "%hd", &charging);
    fclose(ac_online_file);
    return charging;
};

double bat_energy_design(BatteryFileManager * bfmanager) {
    double energy = 0.0;
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        energy += read_bat_file(bfmanager->energy_design_files[index]);
    }
    return small_float(energy) ? NO_HEALTH_INFO : energy;
};

double bat_energy_full(BatteryFileManager * bfmanager) {
    double energy = 0.0;
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        energy += read_bat_file(bfmanager->energy_full_files[index]);
    }
    return small_float(energy) ? NO_ENERGY_INFO : energy;
};

double bat_energy_now(BatteryFileManager * bfmanager) {
    double energy = 0.0;
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        energy += read_bat_file(bfmanager->energy_now_files[index]);
    }
    return small_float(energy) ? NO_ENERGY_INFO : energy;
};

double read_bat_file(char * fname) {
    FILE* bat_file = fopen(fname, "r");
    if (bat_file == NULL) {
        return NO_ENERGY_INFO;
    }
    double bat_stat;
    fscanf(bat_file, "%lf", &bat_stat);
    fclose(bat_file);
    return bat_stat;
};

double system_uptime() {
    FILE* uptime_file = fopen(UPTIME_FILE, "r");
    if (uptime_file == NULL) {
        return NO_UPTIME;
    }
    double uptime;
    fscanf(uptime_file, "%lf", &uptime);
    fclose(uptime_file);
    return uptime;
};