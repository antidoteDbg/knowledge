#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	pid_t child;
	int status;

	child = fork();

	if(child >= 0)
	{
		if(child == 0)
		{	// this is the child process
			ptrace(PTRACE_TRACEME, 0, NULL, NULL);
			execl(argv[1], NULL, NULL);
		}
		else
		{
			// this is the parent process
			ptrace(PTRACE_SETOPTIONS, child, PTRACE_0_TRACEEXEC, NULL);
			waitpid(child, &status, 0);
		}
	}
	else
	{
		printf("fork() failed.");
	}

	return 0;
}
