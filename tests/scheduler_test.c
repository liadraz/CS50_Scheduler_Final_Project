/*******************************************************************************
****************************** - SCHEDULER - ********************************
*
*	DESCRIPTION		Tests Project Scheduler
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdio.h>		/* printf, puts, size_t */
#include <stdlib.h>		/* abort */

#include "utilities.h" 		/* UNUSED */
#include "scheduler.h"

typedef struct cartoon
{
	char *name;
	char *color;
	time_t interval;
} cartoon_ty;

typedef struct package
{
	scheduler_ty *sched;
	sched_id_ty id;
} package_ty;

/* Global Declaration */
cartoon_ty patrik = {"Patrik", "pink", 1};
cartoon_ty sponge_bob = {"Sponge Bob", "yellow", 2};
cartoon_ty gary = {"Gary", "blue", 3};
cartoon_ty skidward = {"Skidward", "cyan", 5};

void TestSchedCreate(void);
void TestSchedRun(void);
int TestSchedAdd(void);
void TestSchedRemove(void);
void TestSchedPause(void);
void TestSchedSize(void);
void TestSchedIsEmpty(void);
void TestSchedClear(void);

static scheduler_ty *CreateSchedulerWithTasks(void);
static int ExeTask(void *params);
static int PauseTask(void *params);
static int RemoveInRunTask(void *params);

int main(void)
{
	PRINT_MSG(\n--- Tests Project Scheduler ---\n);

	TestSchedCreate();
	TestSchedRun();
	TestSchedAdd();
	TestSchedRemove();
	TestSchedPause();
	TestSchedSize();
	TestSchedIsEmpty();
	TestSchedClear();

	return 0;
}

/*******************************************************************************
********************************** Tests **************************************/

void TestSchedCreate(void)
{
	scheduler_ty *scheduler = SchedCreate();

	if (NULL == scheduler)
	{
		RED;
		PRINT_STATUS_MSG(Test Create: FAILED);
		DEFAULT;

		abort();
	}

	GREEN;
	PRINT_STATUS_MSG(Test Create: SUCCESS);
	DEFAULT;

	SchedDestroy(scheduler);
}


void TestSchedRun(void)
{
	scheduler_ty *scheduler = NULL;
	enum run_status_ty exit_status = -1;

	scheduler = SchedCreate();
	if (NULL == scheduler)
	{
		PRINT_MSG(allocation failure in run);
		return;
	}

	SchedAdd(scheduler, ExeTask, &sponge_bob, sponge_bob.interval);
	SchedAdd(scheduler, ExeTask, &skidward, skidward.interval);

	exit_status = SchedRun(scheduler);

	if (0 == exit_status)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Run: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Run: FAILED);
		DEFAULT;
	}

	SchedDestroy(scheduler);
}


int TestSchedAdd(void)
{
	scheduler_ty *scheduler = NULL;
	sched_id_ty id_task1 = {0};
	sched_id_ty id_task2 = {0};
	sched_id_ty id_task3 = {0};
	sched_id_ty id_task4 = {0};
	size_t counter = 0;

	scheduler = SchedCreate();
	if (NULL == scheduler)
	{
		return -1;
	}

	id_task1 = SchedAdd(scheduler, ExeTask, &patrik, 2);
	id_task2 = SchedAdd(scheduler, ExeTask, &sponge_bob, 3);
	id_task3 = SchedAdd(scheduler, ExeTask, &gary, 4);
	id_task4 = SchedAdd(scheduler, ExeTask, &skidward, 5);

	if (0 == UIDIsBad(id_task1))
	{ ++counter; }

	if (0 == UIDIsBad(id_task2))
	{ ++counter; }

	if (0 == UIDIsBad(id_task3))
	{ ++counter; }

	if (0 == UIDIsBad(id_task4))
	{ ++counter; }

	if (4 == counter)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Add: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Add: FAILED);
		DEFAULT;
	}

	SchedDestroy(scheduler);
	return 0;
}


void TestSchedRemove(void)
{
	scheduler_ty *scheduler = NULL;
	package_ty *pkg = NULL;
	sched_id_ty id_task1 = {0};
	uid_ty random_id = UIDGenerate();
	size_t counter = 0;

	scheduler = SchedCreate();
	if (NULL == scheduler)
	{
		return;
	}
	pkg = (package_ty *)malloc(sizeof(package_ty));
	if (NULL == pkg)
	{
		free (scheduler);
		return;
	}

	id_task1 = SchedAdd(scheduler, ExeTask, &patrik, 2);

	/* 1. Test remove uid that located in pqueue */
	if (0 == SchedRemove(scheduler, id_task1))
	{ ++counter; }

	/* 2. Test uid is not in pqueue */
	if (1 == SchedRemove(scheduler, random_id))
	{ ++counter; }

	/* 3. Test remove the current task */
	pkg->id = SchedAdd(scheduler, RemoveInRunTask, pkg, 3);
	pkg->sched = scheduler;

	if (0 == SchedRun(scheduler))
	{ ++counter; }

	/* Evaluate all tests */
	if (3 == counter)
	{
		GREEN;
		PRINT_STATUS_MSG(Test pqueue_Remove: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test pqueue_Remove: FAILED);
		DEFAULT;
	}

	free(pkg);
	SchedDestroy(scheduler);
}


void TestSchedPause(void)
{
	scheduler_ty *scheduler = NULL;
	enum run_status_ty exit_status = -1;

	scheduler = SchedCreate();
	if (NULL == scheduler)
	{
		PRINT_MSG(allocation failure in run);
		return;
	}

	SchedAdd(scheduler, ExeTask, &sponge_bob, sponge_bob.interval);
	SchedAdd(scheduler, PauseTask, scheduler, 1);

	exit_status = SchedRun(scheduler);

	if (STOPPED == exit_status)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Run: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Run: FAILED);
		DEFAULT;
	}

	SchedDestroy(scheduler);
}


void TestSchedSize(void)
{
	scheduler_ty *scheduler = CreateSchedulerWithTasks();
	size_t size = 0;

	size = SchedSize(scheduler);
	if (4 == size)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Size: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Size: FAILED);
		DEFAULT;
	}

	SchedDestroy(scheduler);
}


void TestSchedIsEmpty(void)
{
	scheduler_ty *scheduler = NULL;
	int is_empty = -1;

	scheduler = SchedCreate();
	if (NULL == scheduler)
	{
		puts("TestSchedIsEmpty: Allocation faild");
		return;
	}

	is_empty = SchedIsEmpty(scheduler);
	if (1 == is_empty)
	{
		GREEN;
		PRINT_STATUS_MSG(Test IsEmpty: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test IsEmpty: FAILED);
		DEFAULT;
	}

	SchedAdd(scheduler, ExeTask, &patrik, 2);
	SchedAdd(scheduler, ExeTask, &skidward, 3);

	is_empty = SchedIsEmpty(scheduler);
	if (1 != is_empty)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Is NOT Empty: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Is NOT Empty: FAILED);
		DEFAULT;
	}

	SchedDestroy(scheduler);
}


void TestSchedClear(void)
{
	scheduler_ty *scheduler = CreateSchedulerWithTasks();
	int is_empty = -1;

	SchedClear(scheduler);
	is_empty = SchedIsEmpty(scheduler);

	if ((1 == is_empty) && (scheduler != NULL))
	{
		GREEN;
		PRINT_STATUS_MSG(Test Clear: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Clear: FAILED);
		DEFAULT;
	}

	SchedDestroy(scheduler);
}

/*-------------------------------Side Functions ------------------------------*/
static scheduler_ty *CreateSchedulerWithTasks(void)
{
	scheduler_ty *ret = NULL;
	sched_id_ty id_task1 = {0};
	sched_id_ty id_task2 = {0};
	sched_id_ty id_task3 = {0};
	sched_id_ty id_task4 = {0};

	ret = SchedCreate();
	if (NULL == ret)
	{
		return NULL;
	}

	id_task1 = SchedAdd(ret, ExeTask, &patrik, 1);
	id_task2 = SchedAdd(ret, ExeTask, &sponge_bob, 2);
	id_task3 = SchedAdd(ret, ExeTask, &gary, 3);
	id_task4 = SchedAdd(ret, ExeTask, &skidward, 6);

	UNUSED(id_task1);
	UNUSED(id_task2);
	UNUSED(id_task3);
	UNUSED(id_task4);

	return ret;
}

static int ExeTask(void *params)
{
	static int runThreeTimes = 0;
	cartoon_ty *cartoon = params;

	++runThreeTimes;

	printf("\tName: %s\tInterval: %lu\tTime: %lu\n", cartoon->name, cartoon->interval, time(NULL));


	return (5 <= runThreeTimes); /* SUCCESS 0*/
}

static int PauseTask(void *scheduler)
{
	static int count = 0;

	++count;
	if (3 <= count)
	{
		SchedPause(scheduler);
	}

	return 0;
}

static int RemoveInRunTask(void *params)
{
	puts("\tNext Task Will remove itself");
	SchedRemove(((package_ty *)params)->sched, ((package_ty *)params)->id);
	return 0;
}

