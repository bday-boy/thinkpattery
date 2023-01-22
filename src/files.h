#ifndef FILES_H_
#define FILES_H_

#define UPTIME_FILE "/proc/uptime"
#define POWER_SUPPLY_DIR "/sys/class/power_supply"
#define AC_DIR "AC"
#define BAT_DIR "BAT"
#define AC_ONLINE_FILE "online"
#define ENERGY_DESIGN "energy_full_design"
#define ENERGY_FULL "energy_full"
#define ENERGY_NOW "energy_now"

#define NO_UPTIME -1.0
#define NO_CHARGING_INFO -1
#define NO_ENERGY_INFO 0.0
#define NO_HEALTH_INFO -1.0
#define MAX_FILENAME_LEN 1024

typedef struct BatteryFileManager {
    unsigned short num_batteries;
    char ** energy_design_files;
    char ** energy_full_files;
    char ** energy_now_files;

    char * ac_online_file;
} BatteryFileManager;

BatteryFileManager * new_battery_file_manager();
void del_battery_file_manager(BatteryFileManager * bfmanager);

// Checks if the AC adapter is connected
short bat_is_charging(BatteryFileManager * bfmanager);

double bat_energy_design(BatteryFileManager * bfmanager);
double bat_energy_full(BatteryFileManager * bfmanager);
double bat_energy_now(BatteryFileManager * bfmanager);

// Gets the first value in /proc/uptime
double system_uptime_in_seconds();

// All battery info files are formatted as a single float followed by either
// nothing or garbage info, so this function just reads the first float as a
// double
double read_first_double(char * fname, double value_when_no_file);

#endif // FILES_H_