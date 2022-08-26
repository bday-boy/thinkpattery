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
    setbuf(stdout, NULL);
    BatteryTracker * tracker = new_tracker();

    struct timespec tim = decimal_to_timespec(SAMPLE_FREQUENCY);

    while (1) {
        update_tracker(tracker);
        printf("%s %.1lf%%\n", icons[4], battery_percent(tracker));

        nanosleep(&tim, NULL);
    };

    return 0;
};
