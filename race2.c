#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void main(){

    int fd, i;

    fd = open("./log.txt", O_RDWR);
    if(fd < 0){
        sleep(5);
        fd = open("./log.txt", O_RDWR | O_CREAT, 0666);    
        if(fd > 0){
            printf("\nProgram2 created log.txt\n"); 
            close(fd);
        } 

    }

}

