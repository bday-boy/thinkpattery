#include <signal.h> // For signal(), SIGUSR1
#include <stdio.h>  // For setbuf()
#include <time.h>   // For nanosleep(), timespec

#include "battery_tracker.h"
#include "common.h"
#include "utils.h"

BatteryTracker * bat;

// Changes printing mode between percent and time left
void switch_mode(int signal_num) {
    if (bat == NULL) {
        return;
    }
    rotate_display_mode(bat);
};

int main() {
    // Stops printf from buffering so we don't need to flush after every call
    setbuf(stdout, NULL);

    // Sets up handling SIGUSR1 to change battery tracker mode
    signal(SIGUSR1, switch_mode);

    bat = new_tracker();
    struct timespec tim = decimal_to_timespec(SAMPLE_FREQUENCY);

    while (1) {
        update_tracker(bat);
        print_info(bat);

        nanosleep(&tim, NULL);
    };

    del_tracker(bat);

    return 0;
};
