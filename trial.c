#include<stdio.h>
#include<unistd.h>

int main(){


      pid_t p0,p1,p2;
      p0=fork();
      if (p0<0)
      {
         printf("ERror: Parent not created");
      }
      
      if (p0==0)
      {
         p1=fork();
       
         if (p1==0 && p0==0)
         {
             printf(" HI I am CHILD, my id is %d\n",getpid());
             printf("HI I am child, my parent id is%d\n", getppid());
         }
        else if  (p0==0 && p1>0){
                 printf("Hi I am parent, my id is %d\n", p1);
                 printf("HI I am parent, my grandparent id is %d\n", getppid());
         }



      }
      else{
         printf(" I am grandparent, my id is%d\n", p0);
         printf(" I am granparent , my child(parent) id is%d\n", getpid());
      }
return 0;
}
