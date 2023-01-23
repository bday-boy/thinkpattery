#include <dirent.h> // For struct dirent, DIR, readdir(), closedir()
#include <stddef.h> // For size_t
#include <stdio.h>  // For fscanf()
#include <stdlib.h> // For malloc(), free()
#include <string.h> // For strncmp()

#include "files.h"

const char * uptime_file = UPTIME_FILE;
const char * ac_dir = AC_DIR;
const char * bat_dir = BAT_DIR;
const char * ac_online_file = AC_ONLINE_FILE;
const char * energy_design = ENERGY_DESIGN;
const char * energy_full = ENERGY_FULL;
const char * energy_now = ENERGY_NOW;
const char * power_supply_dir = POWER_SUPPLY_DIR;

// Checks if a string has a given prefix. Only compares prefix_length number of
// characters.
unsigned short starts_with(char * str_to_check, const char * prefix,
                           size_t prefix_length) {
    return strncmp(str_to_check, prefix, prefix_length) == 0;
};

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

short bat_is_charging(BatteryFileManager * bfmanager) {
    FILE* ac_online_file = fopen(bfmanager->ac_online_file, "r");
    if (ac_online_file == NULL) {
        return NO_INFO;
    }
    short is_charging;
    fscanf(ac_online_file, "%hd", &is_charging);
    fclose(ac_online_file);
    return is_charging;
};

double bat_energy_design(BatteryFileManager * bfmanager) {
    double energy = 0.0;
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        energy += read_first_double(bfmanager->energy_design_files[index]);
    }
    return (energy < 0.0) ? NO_INFO : energy;
};

double bat_energy_full(BatteryFileManager * bfmanager) {
    double energy = 0.0;
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        energy += read_first_double(bfmanager->energy_full_files[index]);
    }
    return (energy < 0.0) ? NO_INFO : energy;
};

double bat_energy_now(BatteryFileManager * bfmanager) {
    double energy = 0.0;
    for (size_t index = 0; index < bfmanager->num_batteries; index++) {
        energy += read_first_double(bfmanager->energy_now_files[index]);
    }
    return (energy < 0.0) ? NO_INFO : energy;
};

double system_uptime_in_seconds() {
    return read_first_double(UPTIME_FILE);
};

double read_first_double(char * fname) {
    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        return NO_INFO;
    }
    double bat_stat;
    fscanf(file, "%lf", &bat_stat);
    fclose(file);
    return bat_stat;
};