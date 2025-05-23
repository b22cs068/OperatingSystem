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
    pid_t pid; // Process ID
};

// Function to simulate process workload
void simulate_process_workload() {
    // Keep the process busy until it's terminated
    while (1) {
        // Simulate some work by looping infinitely
    }
}

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

                // Simulate process execution by sending SIGCONT to resume the process
                kill(processes[i].pid, SIGCONT);

                // Check if the process can be executed for a full quantum
                if (processes[i].remaining_time > quantum) {
                    sleep(quantum); // Simulate the execution for the quantum time
                    processes[i].remaining_time -= quantum;
                    printf("Process %d (PID: %d) executed for %d units.\n", processes[i].id, processes[i].pid, quantum);
                } else {
                    // Process completes its execution in this round
                    sleep(processes[i].remaining_time); // Simulate the remaining time execution
                    printf("Process %d (PID: %d) executed for %d units.\n", processes[i].id, processes[i].pid, processes[i].remaining_time);
                    processes[i].remaining_time = 0;
                }

                // After execution, stop the process by sending SIGSTOP
                kill(processes[i].pid, SIGSTOP);

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

        // Fork to create a new process
        processes[i].pid = fork();

        if (processes[i].pid == 0) {
            // Child process: simulate workload and be ready to execute
            simulate_process_workload();
            exit(0); // The child should never reach this point
        } else if (processes[i].pid > 0) {
            // Parent process: Immediately stop the child process to simulate process control
            kill(processes[i].pid, SIGSTOP);
        } else {
            printf("Fork failed for process %d\n", processes[i].id);
            exit(1);
        }
    }

    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    // Call the Round-Robin scheduling function (parent process)
    round_robin(processes, num_processes, quantum);

    // After the scheduling is done, terminate all child processes
    for (int i = 0; i < num_processes; i++) {
        kill(processes[i].pid, SIGKILL); // Terminate child processes
        waitpid(processes[i].pid, NULL, 0); // Wait for each process to terminate
    }

    return 0;
}
