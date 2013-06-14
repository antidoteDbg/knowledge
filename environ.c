#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char *argv[])
{
	int i;

	for(i = 0; environ[i] != NULL; ++i)
	{
		printf("%s\n", environ[i]);
	}

	getchar();
	return 0;
}

// getenv() - Get the value of a particular environment var
// setenv() - Set a new value to an environment var
