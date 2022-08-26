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
    del_regressor(tracker->regressor);
    free(tracker);
};

void update_tracker(BatteryTracker * tracker) {
    // Update mean energy using simple moving average method
    tracker->energy_now = bat0_energy_now() + bat1_energy_now();
    update_regressor(tracker->regressor, tracker->energy_now);
};

double battery_percent(BatteryTracker * tracker) {
    return 100.0 * (tracker->energy_now / tracker->energy_full);
};

double seconds_until_end(BatteryTracker * tracker) {
    // When laptop is charging, we want to know the remaining seconds until
    // full. When it's discharging, we want to know when it reaches 0.
    double y_val = tracker->energy_full * tracker->is_charging;
    return find_remaining_seconds(tracker->regressor, y_val);
};
