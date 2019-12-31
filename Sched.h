/*******************************************************************************
 *	Infinity Labs R&D 45
 *	Scheduler header file
 *	Created:	21/03/18
 *	Modified:
 *	Reviewed by:	
 ******************************************************************************/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include "PQueue.h"

typedef struct scheduler sch_t;



/*	Arguments:
		void
 *	Action: 
 		Create a new scheduler
 *	Return value:
 		Pointer to a new scheduler.
 */
sch_t *SchCreate(void);

/*	Arguments:
		Pointer to a scheduler
 *	Action:
 		Free the memory associated with the argument scheduler
 *	Return value:
 		Void
 */
void SchDestroy(sch_t *sched);

/*	Arguments:
		sched		- pointer to scheduler
		task_func	- pointer to function which will execute
		input		- data to pass to task_func
		run_time	- time to execute task_func, relative to scheduler start
 *	Action:
 		Add a new task to the scheduler
 *	Return value:
 		a pointer to a task_id_t type variable of the task's ID, or NULL
 		in case of failure.
 */
task_id_t SchAdd(sch_t *sched, task_func_t task_func, void *params, time_t 		
					interval_in_secs);

/*	Arguments:
		sched	- pointer to scheduler
		task	- a pointer to task_id_t type variable of the task to remove from
			 	  the scheduler.
 *	Action:
 		Remove a scheduled task.
 *	Return value:
 		0 - task succesfully removed
 		1 - task not found, not removed
 */
int SchRemove(sch_t *sched, task_id_t task_id);

/*	Arguments:
		sched	- pointer to scheduler
 *	Action:
 		Count the number of tasks in the scheduler
 *	Return value:
 		The number of tasks in the scheduler
 */
size_t SchSize(const sch_t *sched);

/*	Arguments:
		sched	- pointer to scheduler
 *	Action:
 		Validate wether any tasks are in the scheduler
 *	Return value:
 		1 - True, the scheduler is empty
 		0 - False, the scheduler is NOT empty
 */
int SchIsEmpty(const sch_t *sched);

/*	Arguments:
		sched	- pointer to scheduler
 *	Action:
 		Run the scheduler until all tasks have been completed or a stop signal 
 		is recieved. If recurring tasks exist in the scheduler it will run
 		indefinitely until a stop signal is recieved.
 *	Return value:
 		0 - successfully executed all tasks
 		1 - scheduler terminated without completing all tasks.
 */
int SchRun(sch_t *sched);

/*	Arguments:
		sched	- pointer to scheduler
 *	Action:
 		Stop the schuduler from running.
 */
void SchStop(sch_t *sched);

/*	Arguments:
		sched	- pointer to scheduler
 *	Action:
 		Clear all the available tasks in the schuduler.
 */
void SchClear(sch_t *sched);

#endif
