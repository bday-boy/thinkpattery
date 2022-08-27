#ifndef FILES_H_
#define FILES_H_

#define UPTIME "/proc/uptime"

#define PS_DIR "/sys/class/power_supply"

#define AC PS_DIR "/AC"

#define AC_ONLINE AC "/online"

#define BAT0 PS_DIR "/BAT0"
#define BAT1 PS_DIR "/BAT1"

#define BAT0_FULL BAT0 "/energy_full"
#define BAT1_FULL BAT1 "/energy_full"

#define BAT0_NOW BAT0 "/energy_now"
#define BAT1_NOW BAT1 "/energy_now"

#define NO_UPTIME -1.0
#define NO_CHARGING_INFO -1
#define NO_BATTERY_INFO 0.0

// Checks system uptime as a double
double system_uptime();

// Checks if the AC adapter is connected
short is_charging();

// So I don't need to re-use code
double read_bat_file(char * fname);

// Checks a battery's maximum possible energy
double bat0_energy_full();
double bat1_energy_full();

// Checks a battery's current energy
double bat0_energy_now();
double bat1_energy_now();

#endif // FILES_H_