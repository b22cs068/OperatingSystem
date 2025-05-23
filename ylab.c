#include <stdio.h>
#include <unistd.h>

int main(){
  pid_t p;
printf("before fork\n");
p=fork();
if (p==0)
{
printf(" Iam child id%d\n", getpid());
printf("my parent id is %d\n", getppid());
}
else
{
sleep(8);
printf("My child id is %d\n", p);
printf("i am parent havinf id%d\n", getpid());
}
printf("common");
}
