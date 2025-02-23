/*******************************************************************************
**************************** - PRIORITY QUEUE - ********************************
*
*	DESCRIPTION		Tests
*	AUTHOR          Liad Raz
*
*******************************************************************************/

#include <stdio.h>		/* printf, puts */
#include <stdlib.h>		/* abort */
#include <stddef.h>		/* size_t */
#include <string.h>		/* strcmp */

#include "utilities.h"
#include "pqueue.h"

/* Global declarations */
typedef struct celebs
{
	char *name;
	int age;
	int priority;
} celebs_ty;

celebs_ty brittney = {"Britney Spears", 39, 2};
celebs_ty sponge_bob = {"Sponge Bob", 5, 1};
celebs_ty james = {"James Bond", 42, 5};
celebs_ty chan = {"Jackie Chan", 67, 8};


void TestPQueueCreate(void);
void TestPQueueEnqueue(void);
void TestPQueueDequeue(void);
void TestPQueuePeek(void);
void TestPQueueIsEmpty(void);
void TestPQueueSize(void);
void TestPQueueClear(void);
void TestPQueueErase(void);

static int PQCmpObjs(const void *obj1, const void *obj2, const void *priority);
static int AreNamesMatch(const void *struct_name, const void *looked_for_name);
static pqueue_ty *CreatePQueue(void);
static void PrintPQueue(pqueue_ty *pqueue);

int main(void)
{
	pqueue_ty *pqueue = CreatePQueue();
	PRINT_MSG(\n--- Tests Priority Queue ---\n);

	PrintPQueue(pqueue);
	PQueueDestroy(pqueue);

	TestPQueueCreate();
	TestPQueueEnqueue();
	TestPQueueDequeue();
	TestPQueuePeek();
	TestPQueueIsEmpty();
	TestPQueueSize();
	TestPQueueClear();
	TestPQueueErase();

	return 0;
}

/*-------------------------------Test Function-------------------------------*/

void TestPQueueCreate(void)
{
	int key_param = 10;
	pqueue_ty *pqueue = PQueueCreate(PQCmpObjs, (void *)&key_param);

	if (NULL == pqueue)
	{
		RED;
		PRINT_STATUS_MSG(Test Create: FAILED);
		DEFAULT;
		abort();
	}

	PQueueDestroy(pqueue);

	GREEN;
	PRINT_STATUS_MSG(Test Create: SUCCESS);
	DEFAULT;
}


void TestPQueueEnqueue(void)
{
	void *key_param = OFFSETOF(celebs_ty, priority);
	pqueue_ty *pqueue = PQueueCreate(PQCmpObjs, key_param);
	int status = -1;

	status = PQueueEnqueue(pqueue, &brittney);

	if (0 == status)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Enqueue: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Enqueue: FAILED);
		DEFAULT;
	}

	PQueueDestroy(pqueue);
}

void TestPQueueDequeue(void)
{
	void *key_param = OFFSETOF(celebs_ty, priority);
	pqueue_ty *pqueue = PQueueCreate(PQCmpObjs, key_param);

	PQueueEnqueue(pqueue, &brittney);
	PQueueEnqueue(pqueue, &sponge_bob);
	PQueueDequeue(pqueue);
	PQueueDequeue(pqueue);

	if (1 == PQueueIsEmpty(pqueue))
	{
		GREEN;
		PRINT_STATUS_MSG(Test Dequeue: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Dequeue: FAILED);
		DEFAULT;
	}

	PQueueDestroy(pqueue);
}

void TestPQueuePeek(void)
{
	celebs_ty *celebs_name = NULL;
	pqueue_ty *pqueue = CreatePQueue();
	char *name_highest_priority = "Sponge Bob";

	celebs_name = PQueuePeek(pqueue);

	if (name_highest_priority == celebs_name->name)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Peek: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Peek: FAILED);
		DEFAULT;
	}

	PQueueDestroy(pqueue);
}

void TestPQueueIsEmpty(void)
{
	pqueue_ty *pqueue = CreatePQueue();
	PQueueDequeue(pqueue);
	PQueueDequeue(pqueue);
	PQueueDequeue(pqueue);
	PQueueDequeue(pqueue);

	if (1 == PQueueIsEmpty(pqueue))
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

	PQueueDestroy(pqueue);
}

void TestPQueueSize(void)
{
	pqueue_ty *pqueue = CreatePQueue();
	PQueueDequeue(pqueue);
	PQueueDequeue(pqueue);

	if (2 == PQueueSize(pqueue))
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

	PQueueDestroy(pqueue);
}

void TestPQueueClear(void)
{
	pqueue_ty *pqueue = CreatePQueue();
	PQueueClear(pqueue);

	if (1 == PQueueIsEmpty(pqueue) && 0 == PQueueSize(pqueue))
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

	PQueueDestroy(pqueue);
}

void TestPQueueErase(void)
{
	pqueue_ty *pqueue = CreatePQueue();
	void *to_find = NULL;
	char *nameExists ="Britney Spears";

	void *to_not_find = NULL;
	char *nameNotExsits ="Liad";

	to_find = PQueueErase(pqueue, AreNamesMatch, nameExists);
	if (nameExists == ((celebs_ty *)to_find)->name)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Erase: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Erase: FAILED);
		DEFAULT;
	}

	to_not_find = PQueueErase(pqueue, AreNamesMatch, nameNotExsits);
	if (NULL == to_not_find)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Erase When NOT exists: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Erase When NOT exists: FAILED);
		DEFAULT;
	}

	PQueueDestroy(pqueue);
}

/*-------------------------------Side Functions ------------------------------*/

static int PQCmpObjs(const void *obj1, const void *obj2, const void *priority)
{
	size_t priority1 = (size_t)obj1 + (size_t)priority;
	size_t priority2 = (size_t)obj2 + (size_t)priority;

	return (*(int *)priority1 - *(int *)priority2);
}

static int AreNamesMatch(const void *struct_name, const void *looking_for)
{
	/* strcmp returns 0 for sucess, areMatch 1 is sucess */
	return !strcmp(((celebs_ty *)struct_name)->name, looking_for);
}

static pqueue_ty *CreatePQueue(void)
{
	pqueue_ty *pqueue = PQueueCreate(PQCmpObjs, OFFSETOF(celebs_ty, priority));

	PQueueEnqueue(pqueue, &brittney);
	PQueueEnqueue(pqueue, &sponge_bob);
	PQueueEnqueue(pqueue, &james);
	PQueueEnqueue(pqueue, &chan);

	return pqueue;
}

static void PrintPQueue(pqueue_ty *pqueue)
{
	celebs_ty *celeb_name = NULL;
	celebs_ty *celeb_age = NULL;
	celebs_ty *celeb_priority = NULL;

	while (!PQueueIsEmpty(pqueue))
	{
		celeb_name = (celebs_ty *)PQueuePeek(pqueue);
		celeb_age = (celebs_ty *)PQueuePeek(pqueue);
		celeb_priority = (celebs_ty *)PQueuePeek(pqueue);

		printf("Name:\t%s\n", celeb_name->name);
		printf("Age:\t%d\n", celeb_age->age);
		printf("Priority:\t%d\n", celeb_priority->priority);
		puts("\n");

		PQueueDequeue(pqueue);
	}
}





