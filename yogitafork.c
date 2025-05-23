#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h>
int main(){
pid_t p;
printf("CHild is not created yet\n");
p=fork();
int w1, wstatus;
if (p<0)
{
printf("Error");
}

if (p==0)
{
   sleep(2);
  printf("Hello!! I am child  and my id is%d\n", getpid());
  printf(" MY parent has an id of %d\n", getppid());
}

else 
{ 
 w1= wait(&wstatus);
 printf(" Status: %d \n", WIFEXITED((wstatus)));
 printf("MY CHILD HAS AN ID %d\n", p);
 printf(" I am parent with an id of %d\n", getpid());
}

printf(" THis part is present in both parent and child");
}

