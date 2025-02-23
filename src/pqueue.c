/*******************************************************************************
**************************** - PRIORITY QUEUE - ********************************
*
*	DESCRIPTION		Implementation of priority queue
*	AUTHOR          Liad Raz
*
*******************************************************************************/

#include <stdlib.h>			/* malloc, free*/
#include <assert.h>			/* assert */

#include "utilities.h"
#include "sorted_list.h"
#include "pqueue.h"

#define PQASSERT_NOT_NULL(ptr)									\
		assert (NULL != ptr && "Priority Queue is not allocated");

struct pqueue
{
    sortl_ty *sortl;
};


/*******************************************************************************
***************************** PQueue Create ***********************************/
pqueue_ty *PQueueCreate(PQCmpFunc cmp_func_p, const void *cmp_param)
{
	pqueue_ty *priority_queue = {NULL};

	assert (NULL != cmp_func_p && "PQueueCreate: Function pointer is invalid");

	/* allocate pqueue */
	priority_queue = (pqueue_ty *)malloc(sizeof(pqueue_ty));

	/* check allocation failure */
	if (NULL == priority_queue)
	{
		return NULL;
	}

	/* allocate sortl */
	priority_queue->sortl = SortLCreate(cmp_func_p ,cmp_param);

	/* check handle allocation failure */
	if (NULL == priority_queue->sortl)
	{
		free(priority_queue);
		return NULL;
	}

	return priority_queue;
}

/*******************************************************************************
***************************** PQueue Destroy **********************************/
void PQueueDestroy(pqueue_ty *pqueue)
{
	PQASSERT_NOT_NULL(pqueue);

	/* free pqueue */
	SortLDestroy(pqueue->sortl);

	/* break pqueue fields */
    DEBUG_MODE
    (
    	pqueue->sortl = INVALID_PTR;
    )
	free(pqueue);
}

/*******************************************************************************
***************************** PQueue Enqueue **********************************/
int PQueueEnqueue(pqueue_ty *pqueue, void *data)
{
	sortl_itr_ty ret_itr = {NULL};

	PQASSERT_NOT_NULL(pqueue);

	ret_itr = SortLInsert(pqueue->sortl, data);

	/* check if insertion faild */
	return (SortLIsSameIter(ret_itr, SortLEnd(pqueue->sortl)));
}

/*******************************************************************************
***************************** PQueue Dequeue **********************************/
void PQueueDequeue(pqueue_ty *pqueue)
{
	sortl_itr_ty high_priority = {NULL};

 	PQASSERT_NOT_NULL(pqueue);

 	/* get the first valid iterator in list */
 	high_priority = SortLBegin(pqueue->sortl);

 	/* remove the result */
 	SortLRemove(high_priority);
}

/*******************************************************************************
***************************** PQueue Peek *************************************/
void *PQueuePeek(const pqueue_ty *pqueue)
{
 	PQASSERT_NOT_NULL(pqueue);

	return SortLGetData(SortLBegin(pqueue->sortl));
}

/*******************************************************************************
***************************** PQueue IsEmpty **********************************/
int PQueueIsEmpty(const pqueue_ty *pqueue)
{
 	PQASSERT_NOT_NULL(pqueue);

 	return SortLIsEmpty(pqueue->sortl);

}

/*******************************************************************************
***************************** PQueue Count ************************************/
size_t PQueueSize(const pqueue_ty *pqueue)
{
 	PQASSERT_NOT_NULL(pqueue);

	return SortLCount(pqueue->sortl);
}

/*******************************************************************************
***************************** PQueue Clear ************************************/
void PQueueClear(pqueue_ty *pqueue)
{
 	PQASSERT_NOT_NULL(pqueue);

	/* traverse pqueue and dequeue each element until it gets empty */
	while (!PQueueIsEmpty(pqueue))
	{
		PQueueDequeue(pqueue);
	}
}

/*******************************************************************************
***************************** PQueue Erase ************************************/
void *PQueueErase(pqueue_ty *pqueue, const PQIsMatch match_func, void *param)
{
 	sortl_itr_ty begin = {NULL};
 	sortl_itr_ty end = {NULL};
 	sortl_itr_ty to_erase = {NULL};
 	void *ret_data = NULL;

 	PQASSERT_NOT_NULL(pqueue);
	assert (NULL != match_func && "PQueueErase: Function pointer is invalid");

	begin = SortLBegin(pqueue->sortl);
	end = SortLEnd(pqueue->sortl);

	/* get an iterator to a matched element */
	to_erase = SortLFindIf(begin, end, match_func, param);

	/* In case find failed return NULL */
	if (SortLIsSameIter(to_erase, end))
	{
		return NULL;
	}

	/* keep data from to_erase elemet */
	ret_data = SortLGetData(to_erase);

	/* remove the founded element */
	SortLRemove(to_erase);

	return ret_data;
}




