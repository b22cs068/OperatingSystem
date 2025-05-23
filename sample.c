#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main(){
     char buffer[100];
     int fd = open("sampletext.txt", O_RDONLY);

     if (fd==-1)
     {
        perror("Error opening file");
        return 1;
     }

     int bytesRead = read (fd , buffer, sizeof(buffer)-1);
     buffer[bytesRead]='\0';
     printf("File content is%s\n", buffer);
     close (fd);
     return 0;
}