#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

// Structure to hold process information
struct Process {
    int id;
    int burst_time;
    int remaining_time;
    int priority;
    int waiting_time; // To store waiting time
    pid_t pid; // Process ID
};

// Function to simulate process workload
void simulate_process_workload() {
    while (1) {
        // Simulate some work by looping infinitely
    }
}

// Function to sort processes by priority (for Priority Scheduling)
void sort_by_priority(struct Process processes[], int num_processes) {
    struct Process temp;
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].priority > processes[j].priority) {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

// Function to calculate average waiting time
float calculate_avg_waiting_time(struct Process processes[], int num_processes) {
    int total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    return (float)total_waiting_time / num_processes;
}

// Function to implement Round-Robin scheduling
float round_robin(struct Process processes[], int num_processes, int quantum) {
    int time = 0;
    bool done = false;

    // Initialize waiting times
    for (int i = 0; i < num_processes; i++) {
        processes[i].waiting_time = 0;
    }

    // Loop until all processes have completed
    while (!done) {
        done = true;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0) {
                done = false; // At least one process is still running

                kill(processes[i].pid, SIGCONT); // Resume process execution

                // Add time spent in the ready queue to waiting time
                for (int j = 0; j < num_processes; j++) {
                    if (j != i && processes[j].remaining_time > 0) {
                        processes[j].waiting_time += (processes[i].remaining_time > quantum) ? quantum : processes[i].remaining_time;
                    }
                }

                if (processes[i].remaining_time > quantum) {
                    sleep(quantum); // Simulate the execution for the quantum time
                    processes[i].remaining_time -= quantum;
                    printf("Process %d (PID: %d) executed for %d units.\n", processes[i].id, processes[i].pid, quantum);
                } else {
                    sleep(processes[i].remaining_time); // Simulate the remaining time execution
                    printf("Process %d (PID: %d) executed for %d units.\n", processes[i].id, processes[i].remaining_time);
                    processes[i].remaining_time = 0;

                    kill(processes[i].pid, SIGKILL); // Terminate the process
                    printf("Process %d (PID: %d) has been terminated.\n", processes[i].id, processes[i].pid);
                }

                if (processes[i].remaining_time > 0) {
                    kill(processes[i].pid, SIGSTOP); // Stop process
                }

                printf("Remaining burst times: ");
                for (int j = 0; j < num_processes; j++) {
                    printf("P%d: %d ", processes[j].id, processes[j].remaining_time);
                }
                printf("\n");
            }
        }
    }

    // Calculate and return average waiting time
    return calculate_avg_waiting_time(processes, num_processes);
}

// Function to implement Priority-based scheduling
float priority_scheduling(struct Process processes[], int num_processes) {
    int time = 0;

    // Initialize waiting times
    for (int i = 0; i < num_processes; i++) {
        processes[i].waiting_time = time; // Waiting time is the total time before execution
        time += processes[i].burst_time; // After execution, time increases by the burst time
    }

    for (int i = 0; i < num_processes; i++) {
        kill(processes[i].pid, SIGCONT); // Resume process execution

        printf("Process %d executed (priority %d).\n", processes[i].id, processes[i].priority);
        sleep(processes[i].burst_time); // Simulate execution for burst time

        kill(processes[i].pid, SIGKILL); // Terminate the process
        printf("Process %d (PID: %d) has been terminated.\n", processes[i].id, processes[i].pid);
    }

    // Calculate and return average waiting time
    return calculate_avg_waiting_time(processes, num_processes);
}

int main() {
    int num_processes, quantum;
    float avg_waiting_time_rr, avg_waiting_time_ps;

    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    struct Process processes[MAX_PROCESSES], processes_copy[MAX_PROCESSES];

    // Input process burst times and priorities
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1;
        printf("Enter burst time for Process %d: ", processes[i].id);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;

        printf("Enter priority for Process %d (lower number = higher priority): ", processes[i].id);
        scanf("%d", &processes[i].priority);

        processes[i].pid = fork(); // Fork to create a new process

        if (processes[i].pid == 0) {
            simulate_process_workload(); // Child process simulates workload
            exit(0); // Exit to ensure child doesn't execute the rest of the code
        } else if (processes[i].pid > 0) {
            kill(processes[i].pid, SIGSTOP); // Parent stops the child process
        } else {
            printf("Fork failed for process %d\n", processes[i].id);
            exit(1);
        }

        // Make a copy of the processes for Priority Scheduling
        processes_copy[i] = processes[i];
    }

    // Get the time quantum for Round Robin
    printf("Enter the time quantum for Round Robin: ");
    scanf("%d", &quantum);

    // Run Round Robin and calculate average waiting time
    avg_waiting_time_rr = round_robin(processes, num_processes, quantum);
    printf("\nAverage Waiting Time for Round Robin: %.2f\n", avg_waiting_time_rr);

    // Sort the processes by priority for Priority Scheduling
    sort_by_priority(processes_copy, num_processes);

    // Run Priority Scheduling and calculate average waiting time
    avg_waiting_time_ps = priority_scheduling(processes_copy, num_processes);
    printf("\nAverage Waiting Time for Priority Scheduling: %.2f\n", avg_waiting_time_ps);

    // Compare the two scheduling algorithms
    printf("\nComparing the two scheduling algorithms...\n");
    if (avg_waiting_time_rr < avg_waiting_time_ps) {
        printf("Round Robin performed better with a lower average waiting time.\n");
    } else if (avg_waiting_time_ps < avg_waiting_time_rr) {
        printf("Priority Scheduling performed better with a lower average waiting time.\n");
    } else {
        printf("Both algorithms performed equally in terms of average waiting time.\n");
    }

    // Ensure all child processes have been terminated
    for (int i = 0; i < num_processes; i++) {
        waitpid(processes[i].pid, NULL, 0); // Wait for each process to terminate
    }

    return 0;
}
