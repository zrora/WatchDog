#include "Task.h"

#define SIZEOFTASK sizeof(task_t)

#define CHECKPOINTER(PTR) {\
	if(PTR == NULL)	\
	{\
		return 0;\
	}}

struct task
{	
	task_id_t uid;
	task_func_t task_func;
	void *params;					/* input data for task_func	*/
	time_t interval_in_secs;    	/* time until next run, 0 if not recurring */
	timeval_t next_runtime;
};


task_t *TaskCreate(task_func_t task_func, void *params, time_t interval_in_secs)
{
	task_t* new_task = (task_t*)malloc(SIZEOFTASK);	
	CHECKPOINTER(new_task);

	new_task->uid = UIDCreate();	
	if(1 == UIDIsBad(new_task->uid))
	{
		return NULL;
	}
	new_task->task_func = task_func;
	new_task->params = params;
	new_task->interval_in_secs = interval_in_secs;
	new_task->next_runtime.tv_sec = interval_in_secs;	
	new_task->next_runtime.tv_usec = 0;

	return new_task;
}

void TaskDestroy(task_t *task)
{	
	free(task);
	task = NULL;
}

task_id_t TaskGetID(const task_t *task)
{
	return task->uid;
}

int TaskRun(const task_t *task)
{
	return (task->task_func(task->params));
}

int TaskIsSame(const task_t *task1, const task_t *task2, void *params)
{
	(void)params;
	return (UIDIsSame(task1->uid, task2->uid));
}

timeval_t TaskGetRunTime(const task_t *task)
{
	return task->next_runtime;
}

void TaskUpdateNextRun(task_t *task)
{
	task->next_runtime.tv_sec += task->interval_in_secs ;
}

time_t TaskGetTimeInterval(task_t *task)
{
	return task->interval_in_secs;
}

