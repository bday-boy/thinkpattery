#include <math.h>       // For modf()
#include <stdio.h>      // For setbuf()
#include <time.h>       // For nanosleep(), timespec
#include <pthread.h>    // For pthread_*

pthread_mutex_t update_mutex;

const double ENERGY_SAMPLE_FREQUENCY = 1.0;
const double HEALTH_SAMPLE_FREQUENCY = 5.0;

void init_timespec(struct timespec * tim, double sleep_seconds) {
    double whole_seconds;
    double decimal_seconds = modf(sleep_seconds, &whole_seconds);

    tim->tv_sec = (time_t) whole_seconds;
    tim->tv_nsec = (long int) (decimal_seconds * 1000000000.0);
};

void * update_energy(void * args);
void * update_health(void * args);

int main() {
    // Stops printf from buffering so we don't need to flush after every call
    setbuf(stdout, NULL);

    pthread_t energy_thread, health_thread;

    pthread_create(&energy_thread, NULL, &update_energy, NULL);
    pthread_create(&health_thread, NULL, &update_health, NULL);

    pthread_join(energy_thread, NULL);
    pthread_join(health_thread, NULL);

    return 0;
};

void * update_energy(void * args) {
    struct timespec energy_time;
    init_timespec(&energy_time, ENERGY_SAMPLE_FREQUENCY);
    while (1) {
        pthread_mutex_lock(&update_mutex);
        printf("Updating energy.\n");
        pthread_mutex_unlock(&update_mutex);
        nanosleep(&energy_time, NULL);
    }
};

void * update_health(void * args) {
    struct timespec health_time;
    init_timespec(&health_time, HEALTH_SAMPLE_FREQUENCY);
    while (1) {
        pthread_mutex_lock(&update_mutex);
        printf("Updating health.\n");
        pthread_mutex_unlock(&update_mutex);
        nanosleep(&health_time, NULL);
    }
};
