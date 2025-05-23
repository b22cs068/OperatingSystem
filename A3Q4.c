#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_PROCESSES 100

// Structure to hold process information
struct Process {
    int id;
    int burst_time;
    int priority;
    pid_t pid; // Process ID
};

// Function to simulate process workload
void simulate_process_workload() {
    while (1) {
        // Simulate some work
    }
}

// Function to sort processes based on priority
void sort_by_priority(struct Process processes[], int num_processes) {
    struct Process temp;
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].priority > processes[j].priority) {
                // Swap processes if priority is lower
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

// Function to execute processes based on priority
void priority_scheduling(struct Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        // Simulate process execution by sending SIGCONT to resume the process
        kill(processes[i].pid, SIGCONT);

        // Simulate execution by sleeping for the burst time
        printf("Process %d executed (priority %d).\n", processes[i].id, processes[i].priority);
        sleep(processes[i].burst_time);

        // Terminate the process after execution
        kill(processes[i].pid, SIGKILL);
        printf("Process %d (PID: %d) has been terminated.\n", processes[i].id, processes[i].pid);
    }
}

int main() {
    int num_processes;

    // Taking input from the user
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    struct Process processes[MAX_PROCESSES];

    // Input process burst times and priorities
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1;
        printf("Enter burst time for Process %d: ", processes[i].id);
        scanf("%d", &processes[i].burst_time);
        printf("Enter priority for Process %d (lower number = higher priority): ", processes[i].id);
        scanf("%d", &processes[i].priority);

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

    // Sort processes by priority
    sort_by_priority(processes, num_processes);

    // Execute processes based on priority
    priority_scheduling(processes, num_processes);

    // After the scheduling is done, ensure that all child processes have been terminated
    for (int i = 0; i < num_processes; i++) {
        waitpid(processes[i].pid, NULL, 0); // Wait for each process to terminate
    }

    return 0;
}
