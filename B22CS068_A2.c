 #include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
   pid_t pid1,pid2;
   int w1, w2;
   pid1= fork();
  
   if (pid1==0)
{
   printf("CHild Process 1 is running, id: %d\n", getpid());
   sleep(10);
   printf(" P1 is terminating\n");
   exit(0);
}
pid2=fork();      
if (pid2==0){

    printf("Child Process 2 is running, id: %d\n", getpid());
    sleep (10);
    printf(" P2 is terminating\n");
    exit(0);
}
   int choice;
   printf("1.Enter 1 to make the child 1 wait.\n");
   printf("2.Enter 2 to make the child 2 wait.\n");
   scanf("%d", &choice);
  
if (choice == 1) {
                waitpid(pid1, NULL , 0);  // Wait for P1 to terminate
                
                printf("P0 waited for P1 to terminate.\n");
            }
      if (choice == 2) {
                waitpid(pid2, NULL, 0);  // Wait for P2 to terminate
                printf("P0 waited for P2 to terminate.\n");
            }

}
