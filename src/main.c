#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "battery_tracker.h"
#include "utils.h"

#define BATTERY_CHARGING 0
#define BATTERY_EMPTY 1
#define BATTERY_QUARTER 2
#define BATTERY_HALF 3
#define BATTERY_THREE_QUARTERS 4
#define BATTERY_FULL 5

const char * icons[] = {"", "", "", "", "", ""};

int main() {
    // Stops printf from buffering
    setbuf(stdout, NULL);

    BatteryTracker * tracker = new_tracker();
    tracker->mode = REMAINING_TIME_MODE;
    struct timespec tim = decimal_to_timespec(SAMPLE_FREQUENCY);

    while (1) {
        update_tracker(tracker);
        print_info(tracker, icons[3]);

        nanosleep(&tim, NULL);
    };

    return 0;
};
