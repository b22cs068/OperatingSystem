#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


#define MAX_PROCESSES 100

struct Process {
    int pid;
    int burst_time;
    int priority;
    int remaining_time;
};
void roundRobinScheduling(int quantum, struct Process processes[], int numProcesses);
void priorityScheduling(int priorities[], int numProcesses);


int main() {
    int n;
   

    printf("Enter the number of processes to create: ");
    scanf("%d", &n);


    struct Process processes[n];

    // Create n processes
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            processes[i].pid = getpid();
            processes[i].burst_time = i*10 + 2; 
            processes[i].priority = i%10 + 1;   

           
            printf("Process %d created: PID = %d, Burst Time = %d, Priority = %d\n",  i + 1, processes[i].pid, processes[i].burst_time, processes[i].priority);
            
            // Exit child process
            exit(0);
        }     }



     //TASK 2:
    int choice;
    int quantum, numProcesses;
   int *priorities = NULL; // Declare pointer for priorities array
    printf("Choose a Scheduling Algorithm:\n");
    printf("1. Round-Robin (RR)\n");
    printf("2. Priority-based Scheduling\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

switch (choice) {
        case 1:
            printf("You selected Round-Robin Scheduling.\n");
            printf("Enter the quantum time: ");
            scanf("%d", &quantum);
            roundRobinScheduling(quantum);
            break;

        case 2:
            printf("You selected Priority-based Scheduling.\n");
            printf("Enter the number of processes: ");
            scanf("%d", &numProcesses);

            // Dynamically allocate memory for the priorities array
            priorities = (int *)malloc(numProcesses * sizeof(int));
            if (priorities == NULL) {
                printf("Memory allocation failed.\n");
                exit(1);
            }

            for (int i = 0; i < numProcesses; i++) {
                printf("Enter priority for process %d: ", i + 1);
                scanf("%d", &priorities[i]);
            }
            priorityScheduling(priorities, numProcesses);

            // Free the dynamically allocated memory
            free(priorities);
            
            break;

        default:
            printf("Invalid choice. Please restart the program and choose 1 or 2.\n");
            exit(1);
}
   return 0;
}
    //void roundRobinScheduling(int quantum) {
    // Placeholder function logic for Round-Robin Scheduling
  //  printf("Simulating Round-Robin Scheduling with quantum time: %d\n", quantum);
    // Implement the actual scheduling logic here
//}

// Function to simulate Priority-based Scheduling
//void priorityScheduling(int priorities[], int numProcesses) {
    // Placeholder function logic for Priority-based Scheduling
  //  printf("Simulating Priority-based Scheduling with the following priorities:\n");
    //for (int i = 0; i < numProcesses; i++) {
      //  printf("Process %d: Priority %d\n", i + 1, priorities[i]);
    //}
    // Implement the actual scheduling logic here
// TASK 3
// Function to implement Round-Robin Scheduling
void roundRobinScheduling(int quantum, struct Process processes[], int numProcesses) {
    int front = 0, rear = 0;
    struct Process queue[MAX_PROCESSES];
    int completed = 0;

    // Initialize the queue with all processes
    for (int i = 0; i < numProcesses; i++) {
        queue[rear++] = processes[i];
    }

    // Round-Robin Scheduling
    while (completed < numProcesses) {
        struct Process current = queue[front++];
        if (current.remaining_time > quantum) {
            printf("Process %d executed for %d units.\n", current.pid, quantum);
            current.remaining_time -= quantum;
            queue[rear++] = current; // Re-add to the queue
        } else {
            printf("Process %d executed for %d units.\n", current.pid, current.remaining_time);
            current.remaining_time = 0;
            completed++;
        }

        // Print remaining burst times
        printf("Remaining burst times: ");
        for (int i = front; i < rear; i++) {
            printf("Process %d: %d ", queue[i].pid, queue[i].remaining_time);
        }
        printf("\n");

        // If the queue becomes empty, reset front and rear
        if (front >= rear) {
            front = rear = 0;
            for (int i = 0; i < numProcesses; i++) {
                if (processes[i].remaining_time > 0) {
                    queue[rear++] = processes[i];
                }
            }
        }
    }
}

// Function to simulate Priority-based Scheduling
void priorityScheduling(int priorities[], int numProcesses) {
    // Placeholder function logic for Priority-based Scheduling
    printf("Simulating Priority-based Scheduling with the following priorities:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process %d: Priority %d\n", i + 1, priorities[i]);
    }
    // Implement the actual scheduling logic here
}


