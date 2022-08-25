#include <stdio.h>
#include <stdint.h>

#include "files.h"

// Currently just test code for files.c
int main() {
    uint8_t charging = is_charging();
    double bat0_full = bat0_energy_full();
    double bat1_full = bat1_energy_full();
    double bat0_now = bat0_energy_now();
    double bat1_now = bat1_energy_now();

    printf("Is charging:       %d\n", charging);

    printf("BAT0 energy full:  %lf\n", bat0_full);
    printf("BAT1 energy full:  %lf\n", bat1_full);
    printf("BAT0 energy now:   %lf\n", bat0_now);
    printf("BAT1 energy now:   %lf\n", bat1_now);

    printf("Full energy total: %lf\n", bat0_full + bat1_full);
    printf("Now energy total:  %lf\n", bat0_now + bat1_now);

    return 0;
};
