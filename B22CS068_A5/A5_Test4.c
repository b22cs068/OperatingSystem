#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep
#include <stdbool.h> // For boolean

#define NUM_PHILOSOPHERS 5
#define MAX_EAT_COUNT 3  // Each philosopher eats this many times

pthread_mutex_t forks[NUM_PHILOSOPHERS];  // Mutexes for forks
pthread_mutex_t log_mutex;  // Mutex for logging and shared access

void log_status(int philosopher, const char *status) {
    pthread_mutex_lock(&log_mutex);
    printf("Philosopher %d is %s.\n", philosopher + 1, status);
    pthread_mutex_unlock(&log_mutex);
}

void *philosopher(void *num) {
    int id = *(int *)num;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < MAX_EAT_COUNT; i++) {
        // Thinking
        log_status(id, "thinking");
        sleep(1);  // Simulate thinking

        // Pick up forks to eat
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        } else {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        }

        // Eating
        log_status(id, "eating");
        sleep(2);  // Simulate eating

        // Release forks
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);

        log_status(id, "finished eating");
    }
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    pthread_mutex_init(&log_mutex, NULL);  // Initialize log mutex

    // Initialize mutexes for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    pthread_mutex_destroy(&log_mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("All philosophers have completed their meals. Program terminating.\n");
    return 0;
}
