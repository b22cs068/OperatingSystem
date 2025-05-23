#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep
#include <stdbool.h> // For boolean

#define NUM_PHILOSOPHERS 5
#define MAX_EAT_COUNT 3  // Each philosopher eats 3 times

pthread_mutex_t forks[NUM_PHILOSOPHERS];  // Mutexes representing forks
bool is_eating[NUM_PHILOSOPHERS] = {false}; // Tracks eating status

// Mutex for logging consistent eating/waiting status
pthread_mutex_t log_mutex;

void *philosopher(void *num) {
    int id = *(int *)num;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < MAX_EAT_COUNT; i++) {
        printf("Philosopher %d is thinking.\n", id + 1);  // 1-indexed
        sleep(1);

        // Pick forks following the deadlock-avoidance strategy
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        } else {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        }

        // Eating phase
        pthread_mutex_lock(&log_mutex);
        is_eating[id] = true;  // Mark philosopher as eating
        pthread_mutex_unlock(&log_mutex);

        // Log the eating/waiting status of all philosophers
        pthread_mutex_lock(&log_mutex);
        printf("\nCurrent Status:\n");
        for (int j = 0; j < NUM_PHILOSOPHERS; j++) {
            if (is_eating[j]) {
                printf("Philosopher %d is eating.\n", j + 1);  // 1-indexed
            } else {
                printf("Philosopher %d is waiting.\n", j + 1);  // 1-indexed
            }
        }
        pthread_mutex_unlock(&log_mutex);

        sleep(2);  // Simulate eating

        // Release forks and mark as not eating
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);

        pthread_mutex_lock(&log_mutex);
        is_eating[id] = false;
        pthread_mutex_unlock(&log_mutex);

        printf("Philosopher %d has finished eating.\n", id + 1);
        sleep(1);  // Allow others to eat
    }
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    pthread_mutex_init(&log_mutex, NULL);  // Initialize logging mutex

    // Initialize mutexes for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    pthread_mutex_destroy(&log_mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("\nAll philosophers have eaten enough. Program terminating.\n");
    return 0;
}
