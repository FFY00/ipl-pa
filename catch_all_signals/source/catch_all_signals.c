#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "arguments.h"

#define log(prefix, format, ...) \
	printf("["prefix"] "format"\n", ##__VA_ARGS__)
#define log_stderr(prefix, format, ...) \
	printf("["prefix"] "format"\n", ##__VA_ARGS__)

#define log_error(format, ...) \
	log_stderr("ERROR", format, ##__VA_ARGS__)
#define log_warn(format, ...) \
	log("WARNING", format, ##__VA_ARGS__)
#define log_info(format, ...) \
	log("INFO", format, ##__VA_ARGS__)

#define log_pid(format, ...) \
	log("PID:%d", format, getpid(), ##__VA_ARGS__)

#define r_assert(ret, format, ...) \
	if (ret) { log_error(format, ##__VA_ARGS__); exit(ret); }
#define c_assert(val, ret, format, ...) \
	if (val) { log_error(format, ##__VA_ARGS__); exit(ret); }


int end = 0;

void signal_handler(int signal, siginfo_t *info, void *ptr)
{
	log_pid("Got signal '%s' (%d) from process '%d'",
		strsignal(signal), signal,
		info->si_pid);

	if (signal == SIGINT)
	{
		printf("Got SIGINT -- terminating\n");
		++end;
	}
}

int main(int argc, char** argv)
{
	struct gengetopt_args_info args_info;
	struct sigaction action;
	int i;

	r_assert(cmdline_parser(argc, argv, &args_info),
		 "Error parsing command line arguments.");

	/* arguments sanity check */
	c_assert(args_info.min_arg < 1, EINVAL,
		 "The 'minimum' argument must be 1 or higher.");
	c_assert(args_info.max_arg > 64, EINVAL,
		 "The 'maximum' argument must be 64 or lower.");

	memset(&action, 0, sizeof(action));
	action.sa_sigaction = signal_handler;
	action.sa_flags = SA_SIGINFO;

	log_info("Processing signal from [MIN:%u, MAX:%u]", args_info.min_arg, args_info.max_arg);

	for (i = args_info.min_arg; i <= args_info.max_arg; i++)
		if(sigaction(i, &action, NULL))
			log_warn("Can't install handler for signal '%s' (%d)",
				 strsignal(i), i);

	i = getpid();
	log_info("Terminate: kill -s SIGKILL %d | kill -s SIGINT %d", i, i);

	while(!end)
	{
		printf("Waiting for a signal\n");
		pause();
	}

	cmdline_parser_free(&args_info);
	return 0;
}
