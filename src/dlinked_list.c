
/*******************************************************************************
**************************** - DLINKED_LIST - **********************************
*
*	DESCRIPTION		Implementation Doubly Linked List
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdlib.h>			/* malloc, free*/
#include <assert.h>			/* assert */

#include "dlinked_list.h"

#define ASSERT_WHEN_NULL(ptr)								\
		assert (NULL != ptr && "DLIST is not allocated")
#define ASSERT_NOT_DUMMY(itr) 							\
		assert(&((itr.dlist)->dummy) != itr.to_node			\
		&& "Impossible to invoke function on an empty list");

#define IS_END(pointer) (pointer != INVALID_PTR)

struct node
{
    void *data;
    node_ty *next;
    node_ty *prev;
};

struct dlist
{
    node_ty dummy; /* points the end of dlist */
};

/*******************************************************************************
***************************** Side-Functions **********************************/

static node_ty *CreateNodeImp(void *data);
static void ConnectNodesImp(node_ty *prev_node, node_ty *curr_node);
static dlist_itr_ty ItrToDummyImp(dlist_itr_ty iterator);

/*******************************************************************************
****************************** DList Create ***********************************/
dlist_ty *DListCreate(void)
{
	dlist_ty *new_dlist = (dlist_ty *)malloc(sizeof(dlist_ty));

	if (NULL == new_dlist)
	{
		return NULL;
	}

	/* Init dummy node;
		next and prev will point at each other
		data BADCOFFEE */
	new_dlist->dummy.data = INVALID_PTR;
	new_dlist->dummy.next = &(new_dlist->dummy);
	new_dlist->dummy.prev = &(new_dlist->dummy);

	return new_dlist;
}

/*******************************************************************************
***************************** DList Destroy ***********************************/
void DListDestroy(dlist_ty *dlist)
{
	node_ty *node_to_free = NULL;
	node_ty *list_holder = NULL;

	ASSERT_WHEN_NULL(dlist);

	/* save the beginning of dlist */
	list_holder = (DListBegin(dlist)).to_node;

	while (IS_END(list_holder->data))
	{
		node_to_free = list_holder;
		list_holder = list_holder->next;

		DEBUG_MODE(
		node_to_free->data = INVALID_PTR;
		node_to_free->next = INVALID_PTR;
		node_to_free->prev = INVALID_PTR;
		)
		free(node_to_free);
	}

	DEBUG_MODE(
	dlist->dummy.data = INVALID_PTR;
	dlist->dummy.next = INVALID_PTR;
	dlist->dummy.prev = INVALID_PTR;
	)
	free(dlist);
}


/*******************************************************************************
***************************** DList IsEmpty ***********************************/
int DListIsEmpty(const dlist_ty *dlist)
{
	ASSERT_WHEN_NULL(dlist);

	/* list is empty when dummy points to itself */
	return (dlist->dummy.next == &(dlist->dummy));
}


/*******************************************************************************
****************************** DList IsSame ***********************************/
int DListIsSameIter(dlist_itr_ty itr1, dlist_itr_ty itr2)
{
	return (itr1.to_node == itr2.to_node);
}

/*******************************************************************************
***************************** DList Insert ************************************/
/*  Insertion occurs before current iterator prev_current <--> new_node <--> curr */
dlist_itr_ty DListInsert(dlist_itr_ty where, void *data)
{
	node_ty *new_node = NULL;
	node_ty *current = NULL;
	dlist_itr_ty ret_itr = {NULL};

	assert (NULL != where.to_node && "Iterator is invalid");

	/* Allocate memory to a new node, and pass its data */
	new_node = CreateNodeImp(data);

	if (NULL == new_node)
	{
		return ItrToDummyImp(where);
	}

	current = where.to_node;

	/* connect one node before current with new_node */
	ConnectNodesImp(current->prev, new_node);
	/* connect new_node with current node */
	ConnectNodesImp(new_node, current);

	/* iterators validation checks */
	assert (new_node->next->prev == new_node);
	assert (current->next->prev == current);

	ret_itr.to_node =  new_node;
	DEBUG_MODE(ret_itr.dlist = where.dlist);

	/* return the iterator refered to new_node */
	return ret_itr;
}


/*******************************************************************************
***************************** DList Remove ************************************/
dlist_itr_ty DListRemove(dlist_itr_ty where)
{
	dlist_itr_ty ret_itr = {NULL};

	assert (NULL != where.to_node
	&& "DListRemove: Iterator is invalid");

	assert ((NULL != where.to_node->next)
	&& "DListRemove: Cannot remove from an Empty Dlist");

	assert ((NULL != where.to_node->prev)
	&& "DListRemove: Cannot remove from an Empty Dlist");

	/* returned iterator will be the one following the element to remove */
	ret_itr = DListNext(where);

	/* Connect the iterators located before and after the one to remove */
	ConnectNodesImp((where.to_node)->prev, (where.to_node)->next);

	DEBUG_MODE(
		where.to_node->data = INVALID_PTR;
		where.to_node->next = INVALID_PTR;
		where.to_node->prev = INVALID_PTR;
	)
	free(where.to_node);

	return ret_itr;
}


/*******************************************************************************
***************************** DList Count *************************************/
size_t DListCount(dlist_ty *dlist)
{
	size_t counter = 0;
	dlist_itr_ty runner = {NULL};
	dlist_itr_ty end = {NULL};

	ASSERT_WHEN_NULL(dlist);

	runner = DListBegin(dlist);
	end = DListEnd(dlist);

	while (!DListIsSameIter(runner, end))
	{
		++counter;
		runner = DListNext(runner);
	}

	return counter;
}


/*******************************************************************************
***************************** DList ForEach ***********************************/
int DListForEach(dlist_itr_ty from, dlist_itr_ty to, ExeFunc exe_func_p, void *param)
{
	int ret_status = 0;
	node_ty *runner = from.to_node;
	node_ty *end_of_range = to.to_node;

	assert (from.dlist == to.dlist
	&& "ForEach: iterators refer to different lists");

	assert (INVALID_PTR != from.to_node->data
	&& "ForEach: from iterator refers to End of dlist");

	assert (NULL != from.to_node->data
	&& "ForEach: to iterator refers to Invalid iterator");

	assert (NULL != exe_func_p
	&& "ForEach: ExeFunc function does not exist");

	while (runner != end_of_range && !ret_status)
	{
		/* apply function on both params */
		ret_status = exe_func_p(runner->data, param);

		runner = runner->next;
	}

	return ret_status; /* 0 For SUCCESS */
}


/*******************************************************************************
***************************** DList Find **************************************/
dlist_itr_ty DListFind(dlist_itr_ty from, dlist_itr_ty to, IsMatchFunc match_func_p, const void *param)
{
	node_ty *runner = from.to_node;
	node_ty *end_of_range = to.to_node;
	dlist_itr_ty ret_itr = {NULL};

	assert (NULL != to.to_node->data
	&& "Find: to iterator refers to Invalid iterator");

	assert (NULL != match_func_p
	&& "Find: IsMatchFunc function does not exist");

	assert (from.dlist == to.dlist
	&& "Find: Iterators come from the same list");

	/* traverse range */
	while (runner != end_of_range)
	{
		/* Check if current node holds matched data */
		if (match_func_p(runner->data, param))
		{
			/* when found return the iterator and the list address */
			ret_itr.to_node = runner;
			DEBUG_MODE(ret_itr.dlist = from.dlist);

			return ret_itr;
		}
		runner = runner->next;
	}

	ret_itr.to_node = end_of_range;
	return ret_itr;
}


/*******************************************************************************
***************************** DList CountMatch ********************************/
size_t DListCountMatch(dlist_itr_ty from, dlist_itr_ty to, IsMatchFunc match_func_p, void *param)
{
	dlist_itr_ty runner_itr = from;
	size_t counter = 0;

	assert (NULL != match_func_p
	&& "ForEach: IsMatchFunc function does not exist");

	assert (from.dlist == to.dlist
	&& "CountMatch: iterators refer to different lists");

	while (!DListIsSameIter(runner_itr, to))
	{
		counter += match_func_p(DListGetData(runner_itr), param);

		runner_itr = DListNext(runner_itr);
	}

	return counter;
}



/*******************************************************************************
***************************** DList Begin *************************************/
dlist_itr_ty DListBegin(dlist_ty *dlist)
{
	dlist_itr_ty begin = {NULL};

	ASSERT_WHEN_NULL(dlist);

	/* begin iterator will refer the first valid node */
	begin.to_node = dlist->dummy.next;

	/* In debug mode, iterator will also keep the list address */
	DEBUG_MODE(begin.dlist = dlist);

	return begin;
}

/*******************************************************************************
***************************** DList END ***************************************/
dlist_itr_ty DListEnd(dlist_ty *dlist)
{
	dlist_itr_ty end = {NULL};

	ASSERT_WHEN_NULL(dlist);

	/* end iterator will refer to dummy node */
	end.to_node = &dlist->dummy;

	/* In debug mode, iterator will also keep the list address */
	DEBUG_MODE(end.dlist = dlist);

	return end;
}
/*******************************************************************************
******************************* DList Next ************************************/
dlist_itr_ty DListNext(dlist_itr_ty where)
{
	assert (NULL != where.to_node && "Iterator is invalid");

	assert(&(where.dlist->dummy) != where.to_node
	&& "DListNext: Cannot invoke next on empty list");

	where.to_node = where.to_node->next;

	return where;
}

/*******************************************************************************
******************************* DList Prev ************************************/
dlist_itr_ty DListPrev(dlist_itr_ty where)
{
	assert (NULL != where.to_node && "Iterator is invalid");

	assert(&(where.dlist->dummy) != where.to_node->prev
	&& "DListPrev: Cannot invoke prev on empty list");

	where.to_node = where.to_node->prev;

	return where;
}


/*******************************************************************************
****************************** DList GetData **********************************/
void *DListGetData(dlist_itr_ty where)
{
	assert (NULL != where.to_node && "Iterator is invalid");

	ASSERT_NOT_DUMMY(where);

	return where.to_node->data;
}


/*******************************************************************************
****************************** DList SetData **********************************/
void DListSetData(dlist_itr_ty where, void *data)
{
	assert (NULL != where.to_node && "Iterator is invalid");

	ASSERT_NOT_DUMMY(where);

	where.to_node->data = data;
}


/*******************************************************************************
****************************** DList Splice ***********************************/
dlist_itr_ty DListSplice(dlist_itr_ty target_where, dlist_itr_ty src_from, dlist_itr_ty src_to)
{
	/* target reference points */
	node_ty *end_connection = target_where.to_node;
	node_ty *begin_connection = target_where.to_node->prev;

	/* define portion to copy start and end points, end is not included */
	node_ty *from = src_from.to_node;
	node_ty *to = src_to.to_node->prev;

	/* define source boundary nodes */
	node_ty *boundary_from = src_from.to_node->prev;
	node_ty *boundary_to = src_to.to_node;

	dlist_itr_ty ret_itr = {NULL};

	/* disconnect the nodes surrounding the portion to remove */
	ConnectNodesImp(boundary_from, boundary_to);

	/* connect the previous node with the first node in the portion*/
	ConnectNodesImp(begin_connection, from);
	/* connect the last node in the portion with target connection point */
	ConnectNodesImp(to, end_connection);

	ret_itr.to_node = from;
	return ret_itr;
}

/*******************************************************************************
****************************** DList PushBack *********************************/
int DListPushBack(dlist_ty *dlist, void *data)
{
	dlist_itr_ty last_element = DListEnd(dlist);
	dlist_itr_ty ret_itr = {NULL};

	ASSERT_WHEN_NULL(dlist);

	/* Add node to the back of the dlist */
	ret_itr = DListInsert(last_element, data);

	/* Check if insert FAILD */
	if (DListIsSameIter(DListEnd(dlist), ret_itr))
	{
		return 1;
	}

	return 0;
}

/*******************************************************************************
****************************** DList PushFront ********************************/
int DListPushFront(dlist_ty *dlist, void *data)
{
	dlist_itr_ty first_element = DListBegin(dlist);
	dlist_itr_ty ret_itr = {NULL};
	int ret_status = 0;

	ASSERT_WHEN_NULL(dlist);

	/* Add node to the beginning of dlist */
	ret_itr = DListInsert(first_element, data);

	/* Check if insert FAILD */
	if (DListIsSameIter(DListEnd(dlist), ret_itr))
	{
		ret_status = 1;
	}

	return ret_status;
}

/*******************************************************************************
****************************** DList PopBack **********************************/
void DListPopBack(dlist_ty *dlist)
{
	dlist_itr_ty end_itr = DListEnd(dlist);

	ASSERT_WHEN_NULL(dlist);

	/* Remove the last node in dlist */
	DListRemove(DListPrev(end_itr));
}

/*******************************************************************************
****************************** DList PopFront *********************************/
void DListPopFront(dlist_ty *dlist)
{
	dlist_itr_ty begin_itr = DListBegin(dlist);

	ASSERT_WHEN_NULL(dlist);

	/* Remove the fits node in dlist */
	DListRemove(begin_itr);
}




/*******************************************************************************
***************************** Util Functions **********************************/
static node_ty *CreateNodeImp(void *data)
{
	node_ty *node = (node_ty *)malloc(sizeof(node_ty));

	if (NULL == node)
	{
		return NULL;
	}

	node->data = data;

	return node;
}

static dlist_itr_ty ItrToDummyImp(dlist_itr_ty dummy_itr)
{
	while (dummy_itr.to_node->data == INVALID_PTR)
	{
		dummy_itr = DListNext(dummy_itr);
	}

	return dummy_itr;
}

/* Discriptive Node Connection -
        +---+---+     			+---+---+
        | back  |	o--next-->	| front |
        |		|	<--prev--o	|		|
        +---+---+     			+---+---+
*/
static void ConnectNodesImp(node_ty *back, node_ty *front)
{
	back->next = front;
	front->prev = back;
}


