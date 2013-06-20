#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>

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
			printf("PTRACE_TRACEME failed.");
		}
		else
			return child;

	}
	else
		printf("fork() failed.");

	return NULL;
}

user_regs getRegisters(pid_t child)
{
	user_regs registers;

	ptrace(PTRACE_GETREGS, child, NULL, &registers);

	return registers;
}

struct
{
	unsigned address;
	unsigned original_data;
	char* comment;
} breakpoint;

void setBreakpoint(pid_t child, int address, char* comment)
{
	struct breakpoint* bpoint;
	unsigned trap;

	bpoint = (struct breakpoint*)malloc(sizeof(struct breakpoint));
	bpoint.address = address;
	bpoint.comment = comment;
	bpoint.original_data = ptrace(PTRACE_PEEKTEXT, child, (void*)address, 0);
	breakpoints[++breakpoint_index] = bpoint; // need a global array

	trap = (bpoint.original_data & 0xFFFFFF00) | 0xCC;
	ptrace(PTRACE_POKETEXT, child, (void*)address, (void*)trap);
}

/*
// Let the child run to the breakpoint and wait for it to reach it

ptrace(PTRACE_CONT, child_pid, 0, 0);

wait(&wait_status);
if (WIFSTOPPED(wait_status)) {
    procmsg("Child got a signal: %s\n", strsignal(WSTOPSIG(wait_status)));
}
else {
    perror("wait");
    return;
}

// See where the child is now
ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
procmsg("Child stopped at EIP = 0x%08x\n", regs.eip);

*/

void continueBreakpoint(pid_t child, struct breakpoint* bpoint, user_regs* registers)
{
	ptrace(PTRACE_POKETEXT, child, (void*)bpoint.address, (void*)bpoint.original_data);
	registers.eip -= 1;
	ptrace(PTRACE_SETREGS, child, 0, &registers);

	// one step
	// setBreakpoint(child, bpoint.address, bpoint.comment);
	ptrace(PTRACE_CONT, child, 0, 0);
}
