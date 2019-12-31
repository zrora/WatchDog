/*******************************************************************************
 *	Infinity Labs R&D 45
 *	Sorted List Header File
 *	
 *	Created:	22/03/18
 *	
 ******************************************************************************/
 
#ifndef __TASK_H
#define __TASK_H

#include "UID.h"					/* suid_t type */

typedef suid_t task_id_t;

	/*
	 * task_func_t returns 
	 *		 0 			- task should RERUN.
	 *		 1 			- task should not RERUN.	 				
	 */
typedef int (*task_func_t)(void *params);
typedef struct task task_t;
typedef struct timeval timeval_t;



/*	Arguments:
		task_func	- pointer to function which will execute
		input		- data to pass to task_func
		run_time	- time to execute task_func, relative to scheduler start
		next_run	- time until next run, 0 if not recurring 
	Action: Create a new Task.
 *	Return:	a pointer to a task_t type task variable or NULL in case of failure.
 */
task_t *TaskCreate(task_func_t task_func, void *params, time_t interval_in_secs);

/*	Arguments: a pointer to a task_t type task variable.
 *	Action: Remove and free the task.
 */
void TaskDestroy(task_t *task);

/*	Arguments: a pointer to a task_t type task variable.
 *	Return: A task_id_t type variable of the task ID.
 */
task_id_t TaskGetID(const task_t *task);

/*	Arguments: a pointer to a task_t type task variable.
 *	Action: Executed the func pointed by task_func.
 *	Return: 
 *  		 0: task executed succesfully, will not be rescheduled.
 *  		 1: task execution failed.	
 */
int TaskRun(const task_t *task);

/*	Arguments: 2 pointers to a task_t type task variables.
 *	Return:  0: the tasks have different UIDs.
 *  		 1: the tasks are the same.	
 */
int TaskIsSame(const task_t *task1, const task_t *task2, void *params);

/*	Arguments: a pointer to a task_t type task variable.
 *	Return: a timeval_t type variable of the task runtime.
 */
timeval_t TaskGetRunTime(const task_t *task);

/*	Arguments:
		task	  - a pointer to a task_t type task variable.
		next_run  - a size_t variable of the seconds between reruns.
	Action: modify the run_time of the task.	
 */
void TaskUpdateNextRun(task_t *task);


#endif
