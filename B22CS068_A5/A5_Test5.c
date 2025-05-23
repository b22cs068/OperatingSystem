#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For sleep
#include <time.h>     // For clock_gettime
#include <stdbool.h>  // For boolean

#define NUM_PHILOSOPHERS 5
#define MAX_EAT_COUNT 15  // Number of times each philosopher will eat

pthread_mutex_t forks[NUM_PHILOSOPHERS];  // Mutexes for forks
pthread_mutex_t log_mutex;                // Mutex for logging

typedef struct {
    int id;
    int eat_count;
    double total_wait_time;
} philosopher_data;

void log_status(int philosopher, const char *status) {
    pthread_mutex_lock(&log_mutex);
    printf("Philosopher %d is %s.\n", philosopher + 1, status);
    pthread_mutex_unlock(&log_mutex);
}

double get_current_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void *philosopher(void *arg) {
    philosopher_data *data = (philosopher_data *)arg;
    int id = data->id;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < MAX_EAT_COUNT; i++) {
        // Start waiting
        double wait_start = get_current_time();

        // Thinking
        log_status(id, "thinking");
        sleep(1);  // Simulate thinking

        // Pick up forks
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        } else {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        }

        // End waiting
        double wait_end = get_current_time();
        data->total_wait_time += (wait_end - wait_start);

        // Eating
        log_status(id, "eating");
        sleep(2);  // Simulate eating

        // Release forks
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);

        log_status(id, "finished eating");
        data->eat_count++;
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    philosopher_data philosophers[NUM_PHILOSOPHERS];

    pthread_mutex_init(&log_mutex, NULL);

    // Initialize mutexes for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].id = i;
        philosophers[i].eat_count = 0;
        philosophers[i].total_wait_time = 0.0;
        pthread_create(&threads[i], NULL, philosopher, &philosophers[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print results
    printf("\n=== Performance Results ===\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        double avg_wait_time = philosophers[i].total_wait_time / philosophers[i].eat_count;
        printf("Philosopher %d ate %d times, average wait time: %.2f seconds.\n",
               i + 1, philosophers[i].eat_count, avg_wait_time);
    }

    // Destroy mutexes
    pthread_mutex_destroy(&log_mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("All philosophers have finished their meals. Program terminating.\n");
    return 0;
}
