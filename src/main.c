#include <stdio.h>
#include <stdint.h>

#include "files.h"

// Currently just test code for files.c
int main() {
    uint8_t charging = is_charging();
    uint32_t bat0_full = bat0_energy_full();
    uint32_t bat1_full = bat1_energy_full();
    uint32_t bat0_now = bat0_energy_now();
    uint32_t bat1_now = bat1_energy_now();


    printf("Is charging:       %d\n", charging);

    printf("BAT0 energy full:  %d\n", bat0_full);
    printf("BAT1 energy full:  %d\n", bat1_full);
    printf("BAT0 energy now:   %d\n", bat0_now);
    printf("BAT1 energy now:   %d\n", bat1_now);

    printf("Full energy total: %d\n", bat0_full + bat1_full);
    printf("Now energy total:  %d\n", bat0_now + bat1_now);

    return 0;
};
