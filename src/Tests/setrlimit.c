// setrlimit.c
//
//    setrlimit STACKLIMIT <command>
//
// Set hard limit on stacksize to STACKLIMIT and execute command.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

// Print message MSG to stderr then exit
#define DIE(msg) fprintf (stderr, msg "\n"), exit (EXIT_FAILURE)


int main (int argc, char *argv[])
{
    if (argc < 3)
	DIE ("usage: setrlimit STACKLIMIT <command>");

    char *stack = argv[1];
    int limit = strtol (stack, &stack, 10);
    if (limit < 0 || *stack)
	DIE ("setrlimit: invalid STACKLIMIT");

    struct rlimit rlim;
    rlim.rlim_cur = limit;
    rlim.rlim_max = limit;
    if (setrlimit (RLIMIT_STACK, &rlim) < 0)
	DIE ("setrlimit: setrlimit failed");

    if (execvp (argv[2], &argv[2]) < 0)
	DIE ("setrlimit: execvp failed");

    return EXIT_FAILURE;
}
