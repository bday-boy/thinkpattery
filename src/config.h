#ifndef CONFIG_H_
#define CONFIG_H_

typedef struct PolybarConfig {
    // Icons to print out
    char * icon_battery_charging;
    char * icon_battery_low;
    char * icon_battery_mid_1;
    char * icon_battery_mid_2;
    char * icon_battery_mid_3;
    char * icon_battery_full;
    char * icon_battery_time_left;
    char * icon_battery_health_good;
    char * icon_battery_health_bad;

    // Thresholds for picking which icons to use
    double battery_low_threshold;
    double battery_full_threshold;
    double battery_healthy_threshold;

    // Battery info printing/updating frequency
    double print_frequency;
    double energy_sample_frequency;
    double health_sample_frequency;

    // Moving average config
    double alpha;
    double initial_samples;
} PolybarConfig;

void init_config(PolybarConfig * config);

#endif // CONFIG_H_