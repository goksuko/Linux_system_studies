/* program to demonstrate SIGPIPE*/

#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



#define BUF_SIZE 5

int main(int argc, char *argv[])
{
    int pfd[2], loop = 0;
    char buf[BUF_SIZE] = {};
    ssize_t numRead;
    /* Pipe file descriptors */

    if (pipe(pfd) == -1)
        exit(-1);

    switch (fork()) {
        case -1:
            exit(-1);
        /* Create the pipe */
        case 0:
            /* Child - reads from pipe */
            /* Write end is unused */
            printf("child: closed write end\n");
            if (close(pfd[1]) == -1)
                exit(-1);
  
            for (;;) {
            /* Read data from pipe, echo on stdout */
                strcpy(buf,"");
                numRead = read(pfd[0], buf, BUF_SIZE);
                if (numRead == -1)
                    exit(-1);
                if (numRead == 0){
                    printf("\n child: parent closed write end so reading stopped\n");
                    break;
                }
                /* End-of-file */
                printf("child: waiting 10 seconds");
                sleep(10);
                printf("\nchild process: data received in child process is (%s)\n",buf);
                printf("\nchild process: closing read end\n");
                close(pfd[0]);
                printf("child: waiting 10 seconds");
                sleep(10);

            }               
            printf("\nchild process: closing read end out of loop\n");
            if (close(pfd[0]) == -1)
                exit(-1);

            break;


        default:

            /* Parent - writes to pipe */
            printf("parent: closed read end");
            if (close(pfd[0]) == -1)
            /* Read end is unused */
               exit(-1);

            printf("\nParent: sending user input string to child process - (%s)\n",argv[1]);
        
            write(pfd[1], argv[1], strlen(argv[1]));
            while(1){
                sleep(2);
                printf("\n parent: about to write data to pipe\n");
                write(pfd[1], "test", 4); // SIG_PIPE is generated if we try to write to pipe which has already closed its read end
            }
            exit(0);

     /* Child will see EOF */
     /* Wait for child to finish */
    }
}


// Based on the terminal output and the code in pipe2.c (which demonstrates SIGPIPE when writing to a pipe whose read end is closed), here's what happened step-by-step. I'll explain the process lifecycle, signals, and why the output changed between the two ps -ef | grep ./a.out commands.

// Key Code Behavior Recap
// Pipe setup: A pipe is created, and fork() spawns a child process.
// Child process:
// Closes the write end (pfd[1]).
// Enters an infinite loop: Reads from the pipe, sleeps 10 seconds, prints the data, closes the read end (pfd[0]), sleeps another 10 seconds, then loops again.
// After closing pfd[0], the next read() call will fail (return -1, error EBADF), causing the child to exit(-1).
// Parent process:
// Closes the read end (pfd[0]).
// Writes the user input string (argv[1], e.g., "HELLLOOOOOOOO") once.
// Enters an infinite loop: Sleeps 2 seconds, prints a message, and tries to write "test" (4 bytes) repeatedly.
// If the pipe's read end is closed (by the child), writing to it triggers a SIGPIPE signal, which terminates the parent by default (unless handled).
// Terminal Output Explanation
// First ps -ef | grep ./a.out:


// root      299845  283960  0 12:04 pts/4    00:00:00 ./a.out HELLLOOOOOOOOroot      299846  299845  0 12:04 pts/4    00:00:00 ./a.out HELLLOOOOOOOO
// PID 299845 (parent): Running, PPID is 283960 (some other process).
// PID 299846 (child): Running, PPID is 299845 (its parent).
// At this point, the parent has just started and written the initial string to the pipe. The child has read it (or is about to), but hasn't closed the read end yet. Both processes are active.
// Second ps -ef | grep ./a.out (shortly after):


// root      299846       1  0 12:04 pts/4    00:00:00 ./a.out HELLLOOOOOOOO
// PID 299845 (parent): Gone (no longer listed). It was terminated.
// PID 299846 (child): Still running, but its PPID is now 1 (init/systemd). This indicates the parent process died, and the child was reparented to the init process.
// The grep process (299985) is just the command itself.
// What Caused This?
// The child read the initial write from the parent, then (inside its loop) closed the pipe's read end (pfd[0]) after sleeping 10 seconds.
// With the read end closed, the pipe is "broken." The parent, still in its while(1) loop, attempted another write(pfd[1], "test", 4). This write failed because there's no reader, triggering SIGPIPE (signal 13), which killed the parent process immediately (default action for unhandled SIGPIPE).
// The child continued running (sleeping for another 10 seconds after closing the read end), which is why it's still visible in the second ps. Eventually, the child's loop tried to read() again on the now-closed pfd[0], causing it to exit(-1) (though this happened after the second ps snapshot).
// The program demonstrates SIGPIPE: Writing to a pipe with no reader causes the writer to be killed. In real code, you'd handle SIGPIPE (e.g., with signal() or sigignore()) to avoid termination.
// Notes
// The output assumes the program was run with an argument like ./a.out HELLLOOOOOOOO.
// If SIGPIPE were handled in the parent (not in this code), it wouldn't terminate.
// To confirm, run the program and check dmesg or logs for SIGPIPE messages, or add signal handling to the parent for testing. If you need code changes (e.g., to handle SIGPIPE), let me know!