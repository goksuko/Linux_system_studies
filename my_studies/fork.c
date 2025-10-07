#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t id = fork();
	if (id == -1)
	{
		perror("fork");
		return (1);
	}
	if (id == 0)
	{
		// Child process
		printf("id = %d, Child process: my PID = %d, Parent PID = %d\n", id, getpid(), getppid());
	}
	else
	{
		// Parent process
		printf("id = %d, Parent process: my PID = %d, I created Child with PID = %d\n", id, getpid(), id);
	}
	return (0);
}