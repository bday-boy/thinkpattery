#include <math.h>       // For modf()
#include <pthread.h>    // For pthread_*
#include <signal.h>     // For signal(), SIGUSR1
#include <stdio.h>      // For setbuf()
#include <stdlib.h>     // For getenv()
#include <time.h>       // For nanosleep(), timespec

#include "battery_tracker.h"
#include "config.h"

BatteryTracker * bat;

// Battery info doesn't change that often so we really don't need to sample
// that much
const double PRINT_FREQUENCY = 5.0;
const double ENERGY_SAMPLE_FREQUENCY = 3.0;
const double HEALTH_SAMPLE_FREQUENCY = 15.0;

// Changes printing mode between percent and time left
void switch_mode(int signal_num) {
    if (bat == NULL) {
        return;
    }
    rotate_display_mode(bat);
};

void init_timespec(struct timespec * tim, double sleep_seconds) {
    double whole_seconds;
    double decimal_seconds = modf(sleep_seconds, &whole_seconds);

    tim->tv_sec = (time_t) whole_seconds;
    tim->tv_nsec = (long int) (decimal_seconds * 1000000000.0);
};

void * print_info_periodically() {
    struct timespec print_period;
    init_timespec(&print_period, PRINT_FREQUENCY);
    while (1) {
        print_info(bat);
        printf("Running print_info.\n");
        nanosleep(&print_period, NULL);
    }
};

void * update_energy_periodically() {
    struct timespec energy_update_period;
    init_timespec(&energy_update_period, ENERGY_SAMPLE_FREQUENCY);
    while (1) {
        update_energy(bat->bat_info);
        printf("Running update_energy.\n");
        nanosleep(&energy_update_period, NULL);
    }
};

void * update_health_periodically() {
    struct timespec health_update_period;
    init_timespec(&health_update_period, HEALTH_SAMPLE_FREQUENCY);
    while (1) {
        update_health(bat->bat_info);
        printf("Running update_health.\n");
        nanosleep(&health_update_period, NULL);
    }
};

int main() {
    // Stops printf from buffering so we don't need to flush after every call
    setbuf(stdout, NULL);

    // Sets up handling SIGUSR1 to change battery tracker mode
    signal(SIGUSR1, switch_mode);

    BatteryInfo bat_info;
    init_bat_info(&bat_info);

    PolybarConfig config;
    init_config(&config);

    bat = new_tracker(&bat_info);
    print_info(bat);

    // Declare thread for printing data
    pthread_t print_thread;

    // Declare threads for updating data
    pthread_t energy_thread, health_thread;

    pthread_create(&print_thread, NULL, &print_info_periodically, NULL);
    pthread_create(&energy_thread, NULL, &update_energy_periodically, NULL);
    pthread_create(&health_thread, NULL, &update_health_periodically, NULL);

    pthread_join(print_thread, NULL);
    pthread_join(energy_thread, NULL);
    pthread_join(health_thread, NULL);

    del_tracker(bat);

    return 0;
};
