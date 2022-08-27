#include <signal.h>
#include <stdio.h>
#include <time.h>

#include "battery_tracker.h"
#include "utils.h"

BatteryTracker * bat;

// Changes printing mode from 
void switch_mode(int signal_num) {
    if (bat == NULL) {
        return;
    }
    bat->mode = !bat->mode;
};

// TODO: Change SAMPLE_FREQUENCY macro to a default val and make SIGUSR
// function change sleep time (slower for time estimation)
int main() {
    // Stops printf from buffering
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

    return 0;
};
