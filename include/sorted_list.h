/*******************************************************************************
******************************** - Sort_LIST - *********************************
*
*	DESCRIPTION		API of Sorted list
*	AUTHOR 			Liad Raz
*	FILES			Sort_list.c Sort_list_test.c Sort_list.h
*
*******************************************************************************/

#ifndef __Sort_LIST_H__
#define __Sort_LIST_H__

#include <stddef.h> 	/* size_t */

#include "dlinked_list.h"

/*******************************************************************************
******************************** Typedefs *************************************/
typedef struct sortl sortl_ty;
typedef struct sortl_itr sortl_itr_ty;


/*******************************************************************************
**************************** Function declarations*****************************/

/*******************************************************************************
* DESCRIPTION	Used in Create, Find functions

* RETURN		0 SUCCESS; POSITIVE value obj1 > obj2; NEGATIVE value obj1 < obj2
*******************************************************************************/
typedef int (*CmpFunc)(const void *object1, const void *object2, const void *cmp_param);

/*******************************************************************************
* DESCRIPTION	Creates a sorted list container.
* RETURN		NULL when memory allocation failed.
				Undefined behavior
				- when p_cmp_func pointer is invalid.
* IMPORTANT	 	User needs to free the allocated container.

* Time Complexity 	O(1)
*******************************************************************************/
sortl_ty *SortLCreate(CmpFunc p_cmp_func, const void *cmp_param);


/*******************************************************************************
* DESCRIPTION	Add and sort a new element to a relevant position.
* RETURN		On failure return iterator to end of range

* Time Complexity 	O(number_of_elements)
*******************************************************************************/
sortl_itr_ty SortLInsert(sortl_ty *list, void *data);


/*******************************************************************************
* DESCRIPTION	Get data of a specifiec element.
* IMPORTANT		Undefined behavior when iterator is out of list range.

* Time Complexity 	O(1)
*******************************************************************************/
void *SortLGetData(sortl_itr_ty iter);


/*******************************************************************************
* DESCRIPTION	Frees sorted list container.

* Time Complexity 	O(number_of_elements)
*******************************************************************************/
void SortLDestroy(sortl_ty *list);


/*******************************************************************************
* DESCRIPTION	Obtain the number of elements in the sorted list.

* Time Complexity 	O(number_of_elements)
*******************************************************************************/
size_t SortLCount(const sortl_ty *list);


/*******************************************************************************
* DESCRIPTION	Get iterator to the first valid element.
* IMPORTANT		Undefined behavior when list is not exists.

* Time Complexity 	O(1)
*******************************************************************************/
sortl_itr_ty SortLBegin(sortl_ty *list);


/*******************************************************************************
* DESCRIPTION	Get iterator to the end of range.
* RETURN		An invalid iterator.
* IMPORTANT		Undefined behavior when list is not exists.

* Time Complexity 	O(1)
*******************************************************************************/
sortl_itr_ty SortLEnd(sortl_ty *list);


/*******************************************************************************
* DESCRIPTION	Get iterator to the next element.
* IMPORTANT		Undefined behavior
				- when list is empty
				- when iterator is out of list range.

* Time Complexity 	O(1)
*******************************************************************************/
sortl_itr_ty SortLNext(sortl_itr_ty iter);


/*******************************************************************************
* RETURN		an iterator to the previous element.
* IMPORTANT		Undefined behavior
				- when list is empty
				- when iterator is out of list range.

* Time Complexity 	O(1)
*******************************************************************************/
sortl_itr_ty SortLPrev(sortl_itr_ty iter);


/*******************************************************************************
* DESCRIPTION	Evalutes two iterators addresses
* RETURN		boolean => 	1 SAME; 0 DIFFERENT.

* Time Complexity 	O(1)
*******************************************************************************/
int SortLIsSameIter(sortl_itr_ty iter1, sortl_itr_ty iter2);


/*******************************************************************************
* DESCRIPTION	Merge elements from donor list and sort them in dest.
* IMPORTANT:	Undefined behavior
*				- when lists are not exist.
*
* Time Complexity 	O(n + m)
*******************************************************************************/
void SortLMerge(sortl_ty *dest, sortl_ty *donor);


/*******************************************************************************
* DESCRIPTION	Match element's data in list with data provided by the user.
* RETURN		Iterator to the first found; If not found iterator to the end.

* Time Complexity 	O(n)
*******************************************************************************/
sortl_itr_ty SortLFind(const sortl_ty *list, const void *data);


/*******************************************************************************
* DESCRIPTION	Checks the existence of elements in the sorted list.
* RETURN 		boolean => 1 IS_EMPTY;	0 NOT_EMPTY

* Time Complexity 	O(1)
*******************************************************************************/
int SortLIsEmpty(const sortl_ty *list);


/*******************************************************************************
* DESCRIPTION	Remove element from sort list and frees it from memory.
* RETURN		An iterator to the following item which has been removed.
* IMPORTANT		The original iterator will be invalidate.

* Time Complexity 	O(number_of_elements)
*******************************************************************************/
sortl_itr_ty SortLRemove(sortl_itr_ty iter);


/*******************************************************************************
* DESCRIPTION	Used in SortLFindIf function
* RETURN		boolean => 1 FOUND;	0 NOT_FOUND
* SIGNATURE		int (*IsMatchFunc)(const void *node_data, const void *param);
*******************************************************************************/

/*******************************************************************************
* DESCRIPTION	Locate data in list using the IsMatchFunc function.
* RETURN		Iterator to a matched element.

* Time Complexity 	O(number_of_elements)
*******************************************************************************/
sortl_itr_ty SortLFindIf(sortl_itr_ty from, sortl_itr_ty to, IsMatchFunc is_match_funcp, void *param);


/*******************************************************************************
*****************>>>>>>  AREA 51 - Restricted AREA <<<<<<**********************/
struct sortl_itr
{
    dlist_itr_ty dlist_itr;
};


#endif /* __Sort_LIST_H__ */


