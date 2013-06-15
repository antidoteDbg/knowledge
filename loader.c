#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

int main(int argc, char* argv[])
{
	pid_t child;

	child = fork();

	if(child >= 0)
	{
		if(child == 0)
		{
			ptrace(PTRACE_TRACEME, 0, NULL, NULL);
			execl(argv[1], NULL, NULL);
		}
		else
		{
			ptrace(PTRACE_SETOPTIONS, pid, PTRACE_0_TRACEEXEC, NULL);
			waitpid(pid, &status, 0);
		}
	}
	else
	{
		printf("fork() failed.");
	}

	return 0;
}
