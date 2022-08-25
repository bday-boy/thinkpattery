#include <stddef.h>

#include "battery_tracker.h"
#include "files.h"

BatteryTracker * new_tracker() {
    BatteryTracker * tracker = malloc (sizeof(BatteryTracker));
    tracker->energy_full = bat0_energy_full() + bat1_energy_full();
    tracker->energy_now = bat0_energy_now() + bat1_energy_now();
    tracker->is_charging = is_charging();
    tracker->regressor = new_regressor(SAMPLE_FREQUENCY);
    return tracker;
};

void del_tracker(BatteryTracker * tracker) {
    free(tracker);
};

void update_data(BatteryTracker * tracker) {
    // Update mean energy using simple moving average method
    tracker->energy_now = bat0_energy_now() + bat1_energy_now();
    update_regressor(tracker->regressor, tracker->energy_now);
};
