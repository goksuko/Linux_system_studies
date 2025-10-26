/* Program to demonstrate Race condition*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void main(){

    int fd, i;
    char  buf[100];
    char  buf2[2]={'\n'};

    fd = open("./log.txt", O_RDWR | O_SYNC);

    for(i = 0 ; i < 99 ; i++){
        buf[i] = 'a';
    }

    for(i = 0; i < 5 ; i++){ 
        (void)write(fd,buf,99);
        (void)write(fd,buf2,1);
    }
    printf("\nProgram1 exited\n"); 
    close(fd); 
}

