#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

// Structure to hold process information
struct Process {
    int id;
    int burst_time;
    int remaining_time;
};

// Function to implement Round-Robin scheduling
void round_robin(struct Process processes[], int num_processes, int quantum) {
    int time = 0;
    bool done = false;

    // Loop until all processes have completed
    while (!done) {
        done = true;

        // Traverse all processes in the circular queue
        for (int i = 0; i < num_processes; i++) {
            // Check if the process has remaining time
            if (processes[i].remaining_time > 0) {
                done = false; // At least one process is still running

                // Check if the process can be executed for a full quantum
                if (processes[i].remaining_time > quantum) {
                    time += quantum;
                    processes[i].remaining_time -= quantum;
                    printf("Process %d executed for %d units.\n", processes[i].id, quantum);
                } else {
                    // Process completes its execution in this round
                    time += processes[i].remaining_time;
                    printf("Process %d executed for %d units.\n", processes[i].id, processes[i].remaining_time);
                    processes[i].remaining_time = 0;
                }

                // Print remaining burst times after each round
                printf("Remaining burst times: ");
                for (int j = 0; j < num_processes; j++) {
                    printf("P%d: %d ", processes[j].id, processes[j].remaining_time);
                }
                printf("\n");
            }
        }
    }
}

int main() {
    int num_processes, quantum;

    // Taking input from the user
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    struct Process processes[MAX_PROCESSES];

    // Input process burst times
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1;
        printf("Enter burst time for Process %d: ", processes[i].id);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time; // Initialize remaining time
    }

    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    // Call the Round-Robin scheduling function
    round_robin(processes, num_processes, quantum);

    return 0;
}
