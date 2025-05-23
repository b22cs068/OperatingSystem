#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep
#include <stdbool.h> // For boolean

#define NUM_PHILOSOPHERS 5
#define MAX_EAT_COUNT 10  // Total times each philosopher should eat

pthread_mutex_t forks[NUM_PHILOSOPHERS];  // Mutexes representing forks
pthread_mutex_t log_mutex;  // Mutex for logging and shared counters

int eat_count[NUM_PHILOSOPHERS] = {0};  // Tracks how many times each philosopher eats

void *philosopher(void *num) {
    int id = *(int *)num;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        printf("Philosopher %d is thinking.\n", id + 1);  // 1-indexed
        sleep(1);

        // Pick up forks in a way that prevents deadlock
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        } else {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        }

        // Critical section: Eating phase
        pthread_mutex_lock(&log_mutex);
        eat_count[id]++;
        printf("Philosopher %d is eating. (Eat count: %d)\n", id + 1, eat_count[id]);
        pthread_mutex_unlock(&log_mutex);

        sleep(2);  // Simulate eating

        // Release forks
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);

        printf("Philosopher %d has finished eating.\n", id + 1);

        // Stop the thread if the philosopher has eaten enough times
        if (eat_count[id] >= MAX_EAT_COUNT) break;
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

    // Log the final eating counts to ensure fairness
    printf("\nFinal Eat Counts:\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("Philosopher %d ate %d times.\n", i + 1, eat_count[i]);
    }

    // Destroy mutexes
    pthread_mutex_destroy(&log_mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("All philosophers have eaten sufficiently. Program terminating.\n");
    return 0;
}

