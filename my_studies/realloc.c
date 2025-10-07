#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd = open("hello.txt", O_RDONLY);
	if (fd == -1)
		return (1);
	// char buf[10] = {0};
	// // char *buf = calloc(sizeof(char), 11);
	// if (!buf)
	// {
	// 	close(fd);
	// 	return (1);
	// }
	// while (1)
	// {
	// 	ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
	// 	if (bytes_read == -1)
	// 	{
	// 		close(fd);
	// 		return (1);
	// 	}
	// 	if (bytes_read == 0)
	// 		break;
	// 	buf[bytes_read] = '\0';
	// 	write(1, buf, bytes_read);
	// 	printf("\nend of a round\n");
	// }
	// close(fd);

	// char *buf = "hello"; // this is a read-only string literal
	// printf("buf before: %s\n", buf);
	// we cannot change read only buffer

	char buf[10] = "hello"; // this is a read-write array stored in stack
	int array[10] = {0};
	printf("buf before: %s\n", buf);
	buf[0] = 'A';
	buf[1] = '\0';
	printf("buf after: %s\n", buf);
	printf("size of buf: %ld\n", sizeof(buf));
	printf("size of arraya: %ld\n", sizeof(array));

	char *test = (char *)malloc(10 * sizeof(char));
	printf("address of test: %p, size of test: %ld\n", test, sizeof(test));
	test = (char *)realloc(test, 20 * sizeof(char));
	printf("address of test after realloc: %p, size of test: %ld\n", test, sizeof(test));
	free(test);

	// free(buf);
	return (0);
}