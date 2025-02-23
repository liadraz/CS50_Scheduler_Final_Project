/*******************************************************************************
****************************** - SCHEDULER - ********************************
*
*	DESCRIPTION		Impl Project Scheduler
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdlib.h>			/* malloc, free*/
#include <time.h>			/* time_t, time*/
#include <unistd.h>			/* sleep */
#include <assert.h>			/* assert */

#include "utilities.h"		/* DEBUG_MODE, OFFSETOF, INVALID_PTR */
#include "pqueue.h"			/* PQueueCreate, PQueueDestroy, PQueuePeek
								PQueueDequeue, PQueueEnqueue, PQueueErase,
								PQueueSize, PQueueIsEmpty */
#include "scheduler.h"
#include <stdio.h>
#define SC_ASSERT_NOT_NULL(ptr)	assert (NULL != ptr \
								&& "SCHEDULER is not allocated");

typedef struct task task_ty;
struct task
{
    TaskFunc	task_func_p;
    void	 	*params;
    time_t	 	interval;
    time_t 		next_run;
    uid_ty 		id;
};

struct scheduler
{
    pqueue_ty 	*tasks;
    time_t 		initial_time;
    task_ty 	*current_task;
    int 		should_run;
};

static int CmpTaskNextRunIMP(const void *t1_, const void *t2_, const void *ignore);
static task_ty *CreateNewTaskIMP(scheduler_ty *sched, TaskFunc exe_task_p, void *params, time_t interval);
static int ExecuteTaskIMP(task_ty *current_task);
static int ReScheduleTaskIMP(scheduler_ty *scheduler, task_ty *task);
static void ClearTasksIMP(scheduler_ty *scheduler);
static int IsIdMatchIMP(const void *task_, const void *searched_id_);
static void BreakSchedulerIMP(scheduler_ty *th_);
static void BreakTaskIMP(task_ty *th_);

/*******************************************************************************
**************************** SchedCreate **************************************/
scheduler_ty *SchedCreate(void)
{
	/* allocate memory for scheduler struct */
	scheduler_ty *sched = (scheduler_ty *)malloc(sizeof(scheduler_ty));

	/* check allocation failure  */
	if (NULL == sched)
	{
		return NULL;
	}

	/* init scheduler fileds */
	sched->tasks = PQueueCreate(CmpTaskNextRunIMP, NULL);

	/* check allocation failure  */
	if (NULL == sched->tasks)
	{
		free(sched);
		return NULL;
	}

	/* init scheduler fields */
	sched->initial_time = 0;
	sched->current_task = NULL;
	sched->should_run = 0;

	return sched;
}


/*******************************************************************************
**************************** SchedDestroy *************************************/
void SchedDestroy(scheduler_ty *scheduler)
{
	SC_ASSERT_NOT_NULL(scheduler);

	/* clear all tasks from pqueue */
	ClearTasksIMP(scheduler);
	/* free the pqueue metadata */
	PQueueDestroy(scheduler->tasks);

	/* DEBUG ONLY */
	BreakSchedulerIMP(scheduler);
    /* free scheduler allocation memory */
	free(scheduler);
}



/*******************************************************************************
******************************** SchedRun *************************************/
enum run_status_ty SchedRun(scheduler_ty *th_)
{
	task_ty *current = NULL;
	time_t exe_time = 0;
	int ret_exe = -1;

	SC_ASSERT_NOT_NULL(th_);
	assert (0 == th_->should_run
	&& "SchedRun: Scheduler is currently running");

	/* Init running condition */
	th_->should_run = 1;
	/* Init starting scheduler time to Absolute time */
	th_->initial_time = time(NULL);

	/* start main loop until pause OR all tasks were removed */
	while ((th_->should_run) && !(PQueueIsEmpty(th_->tasks)))
	{
		/* get the highest priority task */
		current = PQueuePeek(th_->tasks);

		/* calculate the future time the task will be executed */
		exe_time = th_->initial_time + current->next_run;

		/* when exe_time is too early send run to sleep */
		while (time(NULL) < exe_time)
		{
			exe_time = sleep(exe_time - time(NULL));
		}

		/* when its about time remove the task from the pqueue */
		PQueueDequeue(th_->tasks);
		/* Update current task in scheduler member */
		th_->current_task = current;

		/* Execute task */
		ret_exe = ExecuteTaskIMP(current);
		/* In success reshedule task */
		if (0 == ret_exe && NULL != th_->current_task)
		{
			/* In case ReSchedule failued free task */
			if (ReScheduleTaskIMP(th_, current))
			{
				free(current);
			}
		}
		/* Otherwise, remove task */
		else
		{
			/* DEBUG ONLY */
			BreakTaskIMP(current);
			free(current);
		}
	}

	/* when main loop finshed reset all scheduler members */
	th_->current_task = NULL;
	th_->should_run = 0;

	/* when pqueue is empty return 0 */
	return !PQueueIsEmpty(th_->tasks);
}


/*******************************************************************************
******************************** SchedAdd *************************************/
sched_id_ty SchedAdd(scheduler_ty *scheduler, TaskFunc exe_task_p, void *params, time_t interval)
{
	task_ty *new_task = NULL;
	int enqueue_status = -1;

	SC_ASSERT_NOT_NULL(scheduler);
	assert (NULL != exe_task_p && "SchedAdd: Function pointer is invalid");
	assert (0 != interval && "SchedAdd: interval can not be zero");

	/* create new task and init its fields */
	new_task = CreateNewTaskIMP(scheduler, exe_task_p, params, interval);

	/* check whether creation succeed */
	if (NULL == new_task)
	{
		return BAD_UID;
	}

	/* insert task to pqueue */
	enqueue_status = PQueueEnqueue(scheduler->tasks, new_task);

	/* In case failure return BAD_UID */
	if (1 == enqueue_status)
	{
		/* DEBUG ONLY */
		BreakTaskIMP(new_task);
		return BAD_UID;
	}

	return new_task->id;
}

/*******************************************************************************
***************************** SchedRemove *************************************/
int SchedRemove(scheduler_ty *th_, uid_ty to_remove_)
{
	task_ty *ret_task = NULL;
	int is_same = -1;

	SC_ASSERT_NOT_NULL(th_);
	assert (!UIDIsSame(BAD_UID, to_remove_) && "SchedRemove: id is invalid");

	/* check if current task is the one we are looking for */
	is_same = IsIdMatchIMP(th_->current_task, (void *)&to_remove_);
	if (is_same)
	{
		assert (0 != th_->should_run
		&& "Cannot remove current task while scheduler is not running ");

		th_->current_task = NULL;
		/* Actual free occurs in the run function */
		return 0;
	}

	/* In case task is not the current, look for it in the pqueue */
	ret_task = PQueueErase(th_->tasks, IsIdMatchIMP, (void *)&to_remove_);

	/* if not found return failure */
	if (NULL == ret_task)
	{
		return 1;
	}

	/* DEBUG ONLY */
	BreakTaskIMP(ret_task); /* TODO doesn't affect the outside function! */
	free(ret_task);

	return 0;
}


/*******************************************************************************
***************************** SchedPause **************************************/
void SchedPause(scheduler_ty *scheduler)
{
	SC_ASSERT_NOT_NULL(scheduler);

	scheduler->should_run = 0;
}

/*******************************************************************************
***************************** SchedSize ***************************************/
size_t SchedSize(scheduler_ty *scheduler)
{
	SC_ASSERT_NOT_NULL(scheduler);

	return PQueueSize(scheduler->tasks);
}

/*******************************************************************************
**************************** SchedIsEmpty *************************************/
int SchedIsEmpty(scheduler_ty *scheduler)
{
	SC_ASSERT_NOT_NULL(scheduler);

	return PQueueIsEmpty(scheduler->tasks);
}

/*******************************************************************************
**************************** SchedClear ***************************************/
void SchedClear(scheduler_ty *scheduler)
{
	SC_ASSERT_NOT_NULL(scheduler);

	ClearTasksIMP(scheduler);
}


/*******************************************************************************
***************************** Side Functions **********************************/
static int CmpTaskNextRunIMP(const void *t1_, const void *t2_, const void *ignore)
{
	const task_ty *task_pqueue = t1_;
	const task_ty *new_task = t2_;

	UNUSED(ignore);

	if (task_pqueue->next_run > new_task->next_run)
	{
		return 1;
	}
	else if (task_pqueue->next_run < new_task->next_run)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


static task_ty *CreateNewTaskIMP(scheduler_ty *sched, TaskFunc exe_task_p, void *params, time_t interval)
{
	time_t actual_time = 0;

	/* allocate new task */
	task_ty *ret_task = (task_ty *)malloc(sizeof(task_ty));

	/* check allocation failure  */
	if (NULL == ret_task)
	{
		return NULL;
	}

	/* calculate the scheduler time whether it is running or not */
	actual_time = sched->should_run * (time(NULL) - sched->initial_time);

	/* Initi task fields */
	ret_task->task_func_p = exe_task_p;
	ret_task->params = params;
	ret_task->interval = interval;
	ret_task->next_run = actual_time + interval;
	ret_task->id = UIDGenerate();

	return ret_task;
}


static int ExecuteTaskIMP(task_ty *task_)
{
	return (task_->task_func_p(task_->params));
}

static int ReScheduleTaskIMP(scheduler_ty *th_, task_ty *task_)
{
	task_->next_run = (time(NULL) - th_->initial_time) + task_->interval;

	return (PQueueEnqueue(th_->tasks, task_));
}

static void ClearTasksIMP(scheduler_ty *th_)
{
	task_ty *to_remove = NULL;

	/* traverse until pqueue is empty */
	while (!PQueueIsEmpty(th_->tasks))
	{
		/* get first task in pqueue */
		to_remove = PQueuePeek(th_->tasks);

		/* break task fields */
		BreakTaskIMP(to_remove);
		/* remove task */
		free(to_remove);

		/* remove element from pqueue in scheduler */
		PQueueDequeue(th_->tasks);
	}
}

static int IsIdMatchIMP(const void *task_, const void *searched_id_)
{
	/* In case Scheduler is not running current task will be NULL */
	if (task_ == NULL)
	{
		return 0;
	}

	return (UIDIsSame(((task_ty *)task_)->id, *((uid_ty *)searched_id_)));
}

static void BreakSchedulerIMP(scheduler_ty *th_)
{
    DEBUG_MODE
    (
		th_->tasks = INVALID_PTR;
		th_->initial_time = 0;
		th_->current_task = 0;
		th_->should_run = 0;
    ); /* DEBUG ONLY */
}

static void BreakTaskIMP(task_ty *th_)
{
	DEBUG_MODE
	(
		th_->params = INVALID_PTR;
		th_->interval = 0;
		th_->next_run = 0;
		th_->id = BAD_UID;
	) /* DEBUG ONLY */
}
