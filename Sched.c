#include "Sched.h" 	/*current header*/	/*Amit: where's the information box? */
#include "Task.h"
#include <unistd.h>		/*sleep*/

#define CHECKPOINTER(PTR) {\
	if(PTR == NULL)	\
	{\
		return 0;\
	}}
	
struct scheduler
{
	pqueue_t *sch;	/* Amit: vague variable name */
	task_t *current;
	int stop;
};

int IdMatchFunc(const void *uid1, const void *uid2)	
{
	if (1 == UIDIsSame(*(suid_t*)uid1, *(suid_t*)uid2))
	{
		return 1;
	}
	return 0;
}

static int SchIsBefore(const void *node_data, const void *new_data, void *param)
{
	timeval_t node_time = TaskGetRunTime((task_t*)node_data);

	timeval_t new_time =  TaskGetRunTime((task_t*)new_data);
	
	(void)param;
	
	if (node_time.tv_sec > new_time.tv_sec)
	{
		return 0;
	}
	return 1;
}

sch_t *SchCreate(void)
{
	sch_t* main_sch = (sch_t*)malloc(sizeof(sch_t));	
	CHECKPOINTER(main_sch);
	
	main_sch->sch = PQueueCreate(SchIsBefore);
	main_sch->current = NULL;
	main_sch->stop = 0;
	
	return main_sch;
}

void SchDestroy(sch_t *sched)
{
	SchClear(sched);	
	PQueueDestroy(sched->sch);
	free(sched);
}

size_t SchSize(const sch_t *sched)
{
	return PQueueSize(sched->sch);
}

int SchIsEmpty(const sch_t *sched)
{
	return PQueueIsEmpty(sched->sch);
}


void SchClear(sch_t *sched)
{
	while(1 != SchIsEmpty(sched))
	{
		TaskDestroy((PQueueDequeue(sched->sch)));	
	}
}

task_id_t SchAdd(sch_t *sched, task_func_t task_func, void *params, time_t interval_in_secs)
{
	task_t* task = TaskCreate(task_func, params, interval_in_secs);
		
	if(0 != PQueueEnqueue(sched->sch, task, NULL))
	{
		return UIDGetBadUID();
	}

	return TaskGetID(task);	
}

int SchRemove(sch_t *sched, task_id_t task_id)
{
	void *data = NULL; 
	data = PQueueRemove(sched->sch, IdMatchFunc, &task_id);
	
	if (data != NULL)
	{
		TaskDestroy(data);
		data = NULL;
	
		return 0;
	}
		
	if(sched->current != NULL && UIDIsSame(TaskGetID(sched->current), task_id))
	{
		TaskDestroy(sched->current);
		sched->current = NULL; 
		
		return 0;
	}

	return 1;
}

void SchStop(sch_t *sched)
{
	sched->stop = 1;
}



int SchRun(sch_t *sched)
{
	time_t time_now = 0;	
	
	while(1 != sched->stop && 0 == SchIsEmpty(sched))
	{
		sched->current = PQueueDequeue(sched->sch);	
		
	 	sleep(TaskGetRunTime(sched->current).tv_sec - time_now); 
	
		time_now += TaskGetRunTime(sched->current).tv_sec - time_now;	/* Amit: what if sleep exits early? it's best to call a time.h function here to get the real time */
		
		if(1 == TaskRun(sched->current))
		{
			TaskUpdateNextRun(sched->current);
			
			if(0 != PQueueEnqueue(sched->sch, sched->current, NULL))
			{
				return 1;
			}
		}
		
		else
		{
			TaskDestroy(sched->current);
			sched->current = NULL; 
		}
		
	}
	return 0;
}


int SchIsBadTaskID(task_id_t task_id)
{
	return UIDIsBad(task_id);
}































