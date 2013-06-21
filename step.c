#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct user_regs_struct user_regs;

pid_t loadProcess(char* path)
{
	pid_t child = fork();

	if(child >= 0)
	{
		if(child == 0)
		{

			ptrace(PTRACE_TRACEME, 0, NULL, NULL);
			execl(path, NULL, NULL);
			printf("PTRACE_TRACEME failed.\n");
		}
		else
			return child;

	}
	else
		printf("fork() failed.\n");

	return 0;
}

user_regs getRegisters(pid_t child)
{
	user_regs registers;

	ptrace(PTRACE_GETREGS, child, NULL, &registers);

	return registers;
}

void stepDebug(pid_t child)
{
	int status;
	user_regs registers;

	wait(&status);

	while(WIFSTOPPED(status)) 
	{
		registers = getRegisters(child);
		getchar();
		printf("EIP: 0x%08x", registers.eip);

		if(ptrace(PTRACE_SINGLESTEP, child, 0, 0) < 0)
			return;

		wait(&status);
	}
}

int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		pid_t child = loadProcess(argv[1]);
		stepDebug(child);
	}

	return 0;
}
