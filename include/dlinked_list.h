/*******************************************************************************
**************************** - DLINKED_LIST - **********************************
*
*	DESCRIPTION		Doubly Linked List API
*	AUTHOR 			Liad Raz
*	FILES			dlinked_list.c dlinked_list_test.c dlinked_list.h
*
*******************************************************************************/

#ifndef __DLINKED_LIST_H__
#define __DLINKED_LIST_H__

#include <stddef.h> /* size_t */

#include "utilities.h"

/*******************************************************************************
******************************** Typedefs *************************************/

typedef struct dlist dlist_ty;
typedef struct dlist_itr dlist_itr_ty;


/*******************************************************************************
**************************** Function declarations ****************************/

/*******************************************************************************
* DESCRIPTION	Creates a doubly linked list container.
* RETURN 	 	NULL at memory allocation failure
* IMPORTANT	 	User needs to free the allocated container, (Use Destory func).
*
* Time Complexity 	O(1)
*******************************************************************************/
dlist_ty *DListCreate(void);


/*******************************************************************************
* DESCRIPTION	Frees doubly linked list container

* Time Complexity 	O(dlist_size)
*******************************************************************************/
void DListDestroy(dlist_ty *dlist);


/*******************************************************************************
* DESCRIPTION	Checks the existence of elements in the doubly linked list.
* RETURN 		boolean => 1 IS_EMPTY;	0 NOT_EMPTY
*
* Time Complexity 	O(1)
*******************************************************************************/
int DListIsEmpty(const dlist_ty *dlist);


/*******************************************************************************
* DESCRIPTION	Evalutes two iterators addresses
* RETURN		boolean => 	1 SAME; 0 DIFFERENT.

* Time Complexity 	O(1)
*******************************************************************************/
int DListIsSameIter(dlist_itr_ty itr1, dlist_itr_ty itr2);


/*******************************************************************************
* DESCRIPTION	Adds a new element to the dlist consists data from the user
* RETURN		On failure func returns an invalid iterator (iterator to END).
* IMPORTANT	 	The original iterator will be invalidate.
*
* Time Complexity 	O(1); O(dlist_size) on memory allocation FAILURE
*******************************************************************************/
dlist_itr_ty DListInsert(dlist_itr_ty where, void *data);


/*******************************************************************************
* DESCRIPTION	Remove an element from dlist and frees it from memory.
* RETURN 		An iterator to the following item which has been removed.
* IMPORTANT:	Undefined behavior
				- When removing an invalid iterator
				- In case dlist is empty.
*
* Time Complexity 	O(1)
*******************************************************************************/
dlist_itr_ty DListRemove(dlist_itr_ty where);


/*******************************************************************************
 DESCRIPTION	Obtain the number of elements in dlinked list

* Time Complexity 	O(dlist_size)
*******************************************************************************/
size_t DListCount(dlist_ty *dlist);


/*******************************************************************************
* DESCRIPTION	Used in ForEach function
* RETURN		status => 	0 SUCCESS; non-zero value FAILURE
*******************************************************************************/
typedef int (*ExeFunc)(void *data, void *param);

/*******************************************************************************
* DESCRIPTION	Traverse dlist elements on a given range, and execute ExeFunc on them.
* RETURN 		status => 0 SUCCESS; non-zero value FAILURE (returned by ExeFunc)
* IMPORTANT:	Undefined behavior
				- when iterators refer to different lists.
*				- from iterator refers to the end of the dlist.
*				- to iterator refers to invalid iterator.
*
* Time Complexity 	O(range)
*******************************************************************************/
int DListForEach(dlist_itr_ty from, dlist_itr_ty to, ExeFunc p_exe_func, void *func_param);


/*******************************************************************************
* DESCRIPTION	Used in Find function
* RETURN		boolean => 	1 MATCH; 0 DIFFERENT.
*******************************************************************************/
typedef int (*IsMatchFunc)(const void *data, const void *param);

/*******************************************************************************
* DESCRIPTION	Match dlist element data with a data provided by the user.
* RETURN		Iterator to the first found; If not found iterator to the end
*				In Debug mode - Also the address of the searched dlist.
*
* Time Complexity 	O(dlist_size)
*******************************************************************************/
dlist_itr_ty DListFind(dlist_itr_ty from, dlist_itr_ty to, IsMatchFunc match_func_p, const void *param);


/*******************************************************************************
* DESCRIPTION	Count matched dlist elements data with a data provided by the user.
*
* Time Complexity 	O(dlist_size)
*******************************************************************************/
size_t DListCountMatch(dlist_itr_ty from, dlist_itr_ty to, IsMatchFunc cmp_func_p, void *param);


/*******************************************************************************
* DESCRIPTION	Get iterator to the first valid element.
* IMPORTANT	 	If dlist does not exist, return invalid iterator.

* Time Complexity 	O(1)
*******************************************************************************/
dlist_itr_ty DListBegin(dlist_ty *dlist);


/*******************************************************************************
* DESCRIPTION	Get iterator to the end of range.
* RETURN		An invalid iterator.
* IMPORTANT		end function return value remains the same until list is destroyed.
*
* Time Complexity 	O(1)
*******************************************************************************/
dlist_itr_ty DListEnd(dlist_ty *dlist);


/*******************************************************************************
* RETURN		an iterator to the next element.
* IMPORTANT:	Undefined behavior when the provided iterator refers to END.
*
* Time Complexity 	O(1)
*******************************************************************************/
dlist_itr_ty DListNext(dlist_itr_ty where);


/*******************************************************************************
* RETURN		an iterator to the previous element.
* IMPORTANT:	Undefined behavior when iterator refers to the first element.
*
* Time Complexity 	O(1)
*******************************************************************************/
dlist_itr_ty DListPrev(dlist_itr_ty where);


/*******************************************************************************
* DESCRIPTION	Get data of a specifiec element.
* IMPORTANT	 	Undefined behavior when iterator refers to end of dlist.
*
* Time Complexity 	O(1)
*******************************************************************************/
void *DListGetData(dlist_itr_ty where);


/*******************************************************************************
* DESCRIPTION	Change data of a specifiec element.
* IMPORTANT	 	Undefined behavior when iterator refers to end of dlist.
*
* Time Complexity 	O(1)
*******************************************************************************/
void DListSetData(dlist_itr_ty where, void *data);


/*******************************************************************************
* DESCRIPTION	Remove elements from source list, "from" to "to" (not included)
				and place them in target list at "where" location.
* RETURN		An iterator to the start of the newly added portion.
* IMPORTANT:	Undefined behavior
*				- In case target_where iterator is inside the source range.
*				- src_from and src_to are not from the same list.
*				- src_from iterator refers to the end.
*				- src_from iterator is located after src_to.
*
* Time Complexity 	O(1)
*******************************************************************************/
dlist_itr_ty DListSplice(dlist_itr_ty target_where, dlist_itr_ty src_from, dlist_itr_ty src_to);


/*******************************************************************************
* DESCRIPTION	Insert element at the end of the dlist.
* RETURN		non-zero value on memory allocation FAILURE
*
* Time Complexity 	O(1)
*******************************************************************************/
int DListPushBack(dlist_ty *dlist, void *data);


/*******************************************************************************
* DESCRIPTION	Insert element at the beginning of the dlist.
* RETURN		non-zero value on memory allocation FAILURE
*
* Time Complexity 	O(1)
*******************************************************************************/
int DListPushFront(dlist_ty *dlist, void *data);


/*******************************************************************************
* IMPORTANT:	Undefined behavior when pooping from an empty dlist.
*
* Time Complexity 	O(1)
*******************************************************************************/
void DListPopBack(dlist_ty *dlist);


/*******************************************************************************
* IMPORTANT:	Undefined behavior when pooping from an empty dlist.
*
* Time Complexity 	O(1)
*******************************************************************************/
void DListPopFront(dlist_ty *dlistrunner);




/*******************************************************************************
>>>>>>>>>>>>>>>>>>>>>>>>> AREA 51 - Restricted AREA <<<<<<<<<<<<<<<<<<<<<<<<<<*/

typedef struct node node_ty;

struct dlist_itr
{
    node_ty *to_node;

    DEBUG_MODE(dlist_ty *dlist;)

};

#endif /* __DLINKED_LIST_H__ */
