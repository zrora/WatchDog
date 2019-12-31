#ifndef WATCHDOG_H_INCLUDED
#define WATCHDOG_H_INCLUDED
#include <sys/types.h>		/*	pid_t typedef	*/

	/*	MMI (Make Me Immortal)
	 *	Invoke this function to create a watchdog process that will keep the
	 *	calling process alive by re-executing it in case of a crash. The
	 *	re-execution of the program will begin from the first instruction of the
	 *	program.
	 *	Arguments:
	 *		-argc: 	The number of command-line arguments to be passed to the
	 *				calling process at re-execution, including the file name.
	 *		-argv:	The null-terminated array of strings to be used as command-
	 *				line arguments to the process at re-execution. By convention
	 *				argv[0] is the file name of the executable binary.
	 *		-env:	The null-terminated string array of environmental variables
	 *				to be passed to the calling process at re-execution.
	 *	Return value: The PID of the watchdog process.
	 */
pid_t MMI(int argc, const char **argv, const char **env);

	/*	DNR (Do Not Revive)
	 *	Cancel the watchdog process invoked by MMI and stop re-execution of the
	 *	calling process.
	 *		-wd_pid:	The PID of the concurrently running watchdog process.
	 *
	 *	Note:	The canceling of the watchdog process works by invoking SIGUSR2.
	 *			The disposition of this signal is to terminate a process. Care
	 *			should be taken to call this function with the correct PID.
	 */
void DNR(pid_t wd_pid);

#endif
