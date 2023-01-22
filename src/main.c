#include <math.h>   // For modf()
#include <signal.h> // For signal(), SIGUSR1
#include <stdio.h>  // For setbuf()
#include <time.h>   // For nanosleep(), timespec

#include "battery_tracker.h"
#include "common.h"

BatteryTracker * bat;
const double SAMPLE_FREQUENCY = 0.5;

// Changes printing mode between percent and time left
void switch_mode(int signal_num) {
    if (bat == NULL) {
        return;
    }
    rotate_display_mode(bat);
};

struct timespec * init_timespec(struct timespec * tim) {
    double whole_seconds;
    double decimal_seconds = modf(SAMPLE_FREQUENCY, &whole_seconds);

    tim->tv_sec = (time_t) whole_seconds;
    tim->tv_nsec = (long int) (decimal_seconds * 1000000000.0);

    return tim;
};

int main() {
    // Stops printf from buffering so we don't need to flush after every call
    setbuf(stdout, NULL);

    // Sets up handling SIGUSR1 to change battery tracker mode
    signal(SIGUSR1, switch_mode);

    bat = new_tracker();
    struct timespec tim;
    init_timespec(&tim);

    while (1) {
        update_tracker(bat);
        print_info(bat);

        nanosleep(&tim, NULL);
    };

    del_tracker(bat);

    return 0;
};
