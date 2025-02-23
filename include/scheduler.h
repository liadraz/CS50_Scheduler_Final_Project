/*******************************************************************************
****************************** - SCHEDULER - ********************************
*
*	DESCRIPTION		API Project Scheduler - Tasks handling
*	AUTHOR          Liad Raz
*	FILES			scheduler.c scheduler_test.c scheduler.h
*
*******************************************************************************/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h> /* size_t */

#include "uid.h" /* uid_ty, UIDGenerate, UIDIsSame */

typedef struct scheduler scheduler_ty;
typedef uid_ty sched_id_ty;

enum run_status_ty
{
	EMPTY = 0,
	STOPPED = 1
};

/*******************************************************************************
* DESCRIPTION	Creates a new scheduler.
* RETURN		NULL when memory allocation failed.
* IMPORTANT	 	User needs to free the scheduler
*
* Time Complexity 	O(1)
*******************************************************************************/
scheduler_ty *SchedCreate(void);


/*******************************************************************************
* DESCRIPTION	Frees the scheduler and the tasks it contains
*
* Time Complexity 	O(n)
*******************************************************************************/
void SchedDestroy(scheduler_ty *scheduler);


/*******************************************************************************
* DESCRIPTION	Starts the scheduler
* RETURN	 	status => 0 EMPTY; 1 PAUSE
* IMPORTANT		Run cannot invoked when scheduler is empty
*
* Time Complexity 	O(n)
*******************************************************************************/
enum run_status_ty SchedRun(scheduler_ty *scheduler);


/*******************************************************************************
* DESCRIPTION	Used in SchedAdd
* RETURN		status => 0 RE-SCHEDULED;	1 DO-NOT INSERT
*
*******************************************************************************/
typedef int (*TaskFunc)(void *params);

/*******************************************************************************
* DESCRIPTION	Adds task to scheduler regarding its priority value
* RETURN	 	BAD_UID when creation fails
*
* Time Complexity 	O(n)
*******************************************************************************/
sched_id_ty SchedAdd(scheduler_ty *scheduler, TaskFunc add_task, void *params, time_t interval);


/*******************************************************************************
* DESCRIPTION	Removes task from scheduler
* RETURN	 	status => 0 SUCCESS; non-zero value FAILURE
*
* Time Complexity 	O(n)
*******************************************************************************/
int SchedRemove(scheduler_ty *scheduler, uid_ty to_remove);


/*******************************************************************************
* DESCRIPTION	Stops the run of scheduler.
* IMPORTANT	 	Undefined behavior when puasing an invalid scheduler
*
* Time Complexity 	O(n)
*******************************************************************************/
void SchedPause(scheduler_ty *scheduler);


/*******************************************************************************
* DESCRIPTION	Obtain how many tasks occupy scheduler.
*
* Time Complexity 	O(n)
*******************************************************************************/
size_t SchedSize(scheduler_ty *scheduler);


/*******************************************************************************
* DESCRIPTION	Checks if scheduler contains tasks
* RETURN	 	boolean => 1 EMPTY;	0 NOT EMPTY
*
* Time Complexity 	O(n)
*******************************************************************************/
int SchedIsEmpty(scheduler_ty *scheduler);


/*******************************************************************************
* DESCRIPTION	Clears and frees all tasks in scheduler
*
* Time Complexity 	O(n)
*******************************************************************************/
void SchedClear(scheduler_ty *scheduler);


#endif /* __SCHEDULER_H__ */

