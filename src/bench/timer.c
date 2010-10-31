#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	pid_t pid;
	struct timeval before, after;
	int diff;

	if (argc < 2)
		return 0;
	
	argv++;
	
	gettimeofday(&before, NULL);

	pid = fork();
	if (pid == -1)
		return 1;
	else if (pid == 0)
	{
		close(0);
		close(1);
		close(2);
		execvp(argv[0], argv);
		error(1, errno, "Cannot run command %s", argv[0]);
	} else
	{
		wait(NULL);
	}

	gettimeofday(&after, NULL);
	diff = (after.tv_sec - before.tv_sec) * 1000 + 
		(after.tv_usec - before.tv_usec) / 1000;

	printf("%d\n", diff);

	return 0;
}
