/*******************************************************************************
****************************** - SCHEDULER - ********************************
*
*	DESCRIPTION		Simple example of running Tasks
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdio.h>		/* printf, puts, size_t */
#include <stdlib.h>		/* abort */

#include "utilities.h" 		/* UNUSED */
#include "scheduler.h"


/*******************************************************************************
******************************* Declearations  ********************************/

typedef struct cartoon
{
	char *name;
	char *color;
	time_t interval;
} cartoon_ty;


/* Global Parameters Declaration */
cartoon_ty patrik = {"Patrik", "pink", 1};
cartoon_ty sponge_bob = {"Sponge Bob", "yellow", 2};
cartoon_ty gary = {"Gary", "blue", 3};
cartoon_ty skidward = {"Skidward", "cyan", 5};


void ExampleSchedRun(void);

/* FuncTasks */
static int ExeTask(void *params);
static int PauseTasks(void *params);



/*******************************************************************************
********************************** MAIN ***************************************/
int main(void)
{
	PRINT_MSG(\n--- Example Project Scheduler ---\n);

	ExampleSchedRun();

	return 0;
}


/*******************************************************************************
****************************** Implementation *********************************/
void ExampleSchedRun(void)
{
	scheduler_ty *scheduler = NULL;
	enum run_status_ty exit_status = -1;

    sched_id_ty id_task1 = {0};
	sched_id_ty id_task2 = {0};
	sched_id_ty id_task3 = {0};
	sched_id_ty id_task4 = {0};
	sched_id_ty id_pause = {0};


	scheduler = SchedCreate();
	if (NULL == scheduler)
	{
		PRINT_MSG(allocation failure in run);
		return;
	}

	id_task1 = SchedAdd(scheduler, ExeTask, &patrik, patrik.interval);
	id_task2 = SchedAdd(scheduler, ExeTask, &sponge_bob, sponge_bob.interval);
	id_task3 = SchedAdd(scheduler, ExeTask, &gary, gary.interval);
	id_task4 = SchedAdd(scheduler, ExeTask, &skidward, skidward.interval);
    id_pause = SchedAdd(scheduler, PauseTasks, scheduler, 12);

	exit_status = SchedRun(scheduler);
	if (STOPPED == exit_status)
	{
		GREEN;
		PRINT_STATUS_MSG(Example Run: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Example Run: FAILED);
		DEFAULT;
	}


	SchedDestroy(scheduler);

    UNUSED(id_task1);
	UNUSED(id_task2);
	UNUSED(id_task3);
	UNUSED(id_task4);
    UNUSED(id_pause);
}


/*------------------------------- FuncTasks ----------------------------------*/

/* Declaration of FuncTask */
static int ExeTask(void *params)
{
	cartoon_ty *cartoon = params;

	printf("\tI am %s\tInterval: %lu\tTime: %lu\n", cartoon->name, cartoon->interval, time(NULL));
    printf("\n");

	return 0; /* SUCCESS 0*/
}


static int PauseTasks(void *scheduler)
{
    printf("\tStopping Scehduler after 12 seconds\n");
    SchedPause(scheduler);

	return 0;
}