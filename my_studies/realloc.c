#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd = open("hello.txt", O_RDONLY);
	if (fd == -1)
		return (1);
	
	char buf[10] = "hello"; // this is a read-write array stored in stack
	int array[10] = {0};
	printf("buf before: %s\n", buf);
	buf[0] = 'A';
	buf[1] = '\0';
	printf("buf after: %s\n", buf);
	printf("size of buf: %ld\n", sizeof(buf));
	printf("size of array: %ld\n", sizeof(array));

	char *test = (char *)malloc(10 * sizeof(char));
	printf("address of test: %p, size of test: %ld\n", test, sizeof(test));
	test = (char *)realloc(test, 20 * sizeof(char));
	printf("address of test after realloc: %p, size of test: %ld\n", test, sizeof(test));
	free(test);

	// free(buf);
	return (0);
}