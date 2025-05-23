#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct Process {
    int pid;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
};

// Function to create processes and store relevant details
void create_processes(struct Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        int burst_time, priority;

        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &burst_time);
        printf("Enter priority for process %d (lower number means higher priority): ", i+1);
        scanf("%d", &priority);

        int pid = fork();
        if (pid == 0) {
            // Child process: simulate some work
            printf("Child process %d created with PID: %d\n", i+1, getpid());
            exit(0);
        } else {
            // Parent process: store details
            processes[i].pid = pid;
            processes[i].burst_time = burst_time;
            processes[i].priority = priority;
            processes[i].remaining_time = burst_time;
            processes[i].waiting_time = 0;
            printf("Parent process: child %d created with PID: %d\n", i+1, pid);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
}

// Function to compare processes by priority (for sorting)
int compare(const void *a, const void *b) {
    struct Process *processA = (struct Process *)a;
    struct Process *processB = (struct Process *)b;
    return processA->priority - processB->priority;  // Lower priority number comes first
}

// Priority-based scheduling algorithm with waiting time calculation
void priority_scheduling(struct Process processes[], int n) {
    // Sort the processes based on priority
    qsort(processes, n, sizeof(struct Process), compare);

    printf("Processes will be executed in the following order (based on priority):\n");
    int total_waiting_time = 0;

    for (int i = 0; i < n; i++) {
        printf("Process %d with priority %d and burst time %d\n", i+1, processes[i].priority, processes[i].burst_time);
    }

    // Execute processes based on sorted priority
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            // Waiting time is the sum of the burst times of all previous processes
            processes[i].waiting_time = processes[i-1].waiting_time + processes[i-1].burst_time;
        }
        total_waiting_time += processes[i].waiting_time;
        printf("Executing Process %d (PID: %d) with priority %d for %d units. Waiting time: %d\n", 
                i+1, processes[i].pid, processes[i].priority, processes[i].burst_time, processes[i].waiting_time);
    }

    float avg_waiting_time = (float) total_waiting_time / n;
    printf("Average Waiting Time (Priority Scheduling): %.2f\n", avg_waiting_time);
}

// Round-Robin scheduling algorithm with waiting time calculation
void round_robin(struct Process processes[], int n, int quantum) {
    int time = 0;
    int completed = 0;
    int total_waiting_time = 0;
    int burst_remaining[n];

    // Initialize burst remaining times
    for (int i = 0; i < n; i++) {
        burst_remaining[i] = processes[i].burst_time;
    }

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (burst_remaining[i] > 0) {
                if (burst_remaining[i] > quantum) {
                    printf("Process %d executed for %d units.\n", i+1, quantum);
                    time += quantum;
                    burst_remaining[i] -= quantum;
                } else {
                    printf("Process %d executed for %d units and completed.\n", i+1, burst_remaining[i]);
                    time += burst_remaining[i];
                    burst_remaining[i] = 0;
                    completed++;

                    // Waiting time = time - burst_time - waiting_time already accumulated
                    processes[i].waiting_time = time - processes[i].burst_time;
                    total_waiting_time += processes[i].waiting_time;
                }
            }
        }
    }

    float avg_waiting_time = (float) total_waiting_time / n;
    printf("Average Waiting Time (Round-Robin): %.2f\n", avg_waiting_time);
}

// Function for the scheduler menu and performance comparison
void scheduler_menu(struct Process processes[], int n) {
    int choice;
    float rr_avg_waiting_time = 0, priority_avg_waiting_time = 0;

    printf("Choose Scheduling Algorithm:\n");
    printf("1. Round-Robin\n");
    printf("2. Priority Scheduling\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int quantum_time;
        printf("Round-Robin selected.\n");
        printf("Enter quantum time: ");
        scanf("%d", &quantum_time);
        printf("Quantum time set to %d.\n", quantum_time);
        round_robin(processes, n, quantum_time);
    } else if (choice == 2) {
        printf("Priority Scheduling selected.\n");
        priority_scheduling(processes, n);
    } else {
        printf("Invalid choice.\n");
        return;
    }

    // Performance comparison
    if (rr_avg_waiting_time < priority_avg_waiting_time) {
        printf("Round-Robin performed better with a lower average waiting time.\n");
    } else if (priority_avg_waiting_time < rr_avg_waiting_time) {
        printf("Priority Scheduling performed better with a lower average waiting time.\n");
    } else {
        printf("Both scheduling algorithms performed equally.\n");
    }
}

int main() {
    int n;

    // Task 1: Creating processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    // Create processes and store their details
    create_processes(processes, n);

    // Task 2: Scheduler configuration interface
    scheduler_menu(processes, n);

    return 0;
}
