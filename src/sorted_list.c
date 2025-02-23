
/*******************************************************************************
****************************** - SORTED_LIST - ********************************
*
*	DESCRIPTION		Implementation of Sorted list
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdlib.h>			/* malloc, free*/
#include <assert.h>			/* assert */

#include "utilities.h"
#include "sorted_list.h"

#define ASSERT_NOT_NULL_IMP(ptr)								\
		assert (NULL != ptr && "Sort LIST is not allocated");


struct sortl
{
    dlist_ty *dlist;
	CmpFunc p_cmp_func;
    const void *cmp_param;
};

typedef struct callback_params_sl
{
	CmpFunc cmp_func_p;
	const void *cmp_param;
	void *user_data;
} callback_params_sl_ty;


/*******************************************************************************
***************************** Side-Functions **********************************/
int IsBiggerImp(const void *element_data, const void *param);
int IsEqualImp(const void *element_data, const void *param);

/*******************************************************************************
***************************** SortL Create ************************************/
sortl_ty *SortLCreate(const CmpFunc cmp_func_p, const void *cmp_param)
{
	sortl_ty *sort_list = NULL;

	assert (NULL != cmp_func_p && "Function pointer is invalid");

	/* allocate sortl */
	sort_list = (sortl_ty *)malloc(sizeof(sortl_ty));

	/* check handle allocation failure */
	if (NULL == sort_list)
	{
		return NULL;
	}

	/* allocate dlist; first member in sortl */
	sort_list->dlist = DListCreate();

	/* check handle allocation failure */
	if (NULL == sort_list->dlist)
	{
		free(sort_list);
		return NULL;
	}

	/* init slist fields */
	sort_list->p_cmp_func = cmp_func_p;
	sort_list->cmp_param = cmp_param;

	return sort_list;
}

/*******************************************************************************
***************************** SortL Insert ************************************/
sortl_itr_ty SortLInsert(sortl_ty *sort_list, void *data)
{
	callback_params_sl_ty callback_params = {NULL};
	IsMatchFunc is_bigger_p = IsBiggerImp;
	sortl_itr_ty begin = {NULL};
	sortl_itr_ty end = {NULL};
	sortl_itr_ty return_itr = {NULL};

    /* debug only */
	ASSERT_NOT_NULL_IMP(sort_list);

	/* fill cmp_objects_package fields with comparison information */
	callback_params.cmp_func_p = sort_list->p_cmp_func;
	callback_params.cmp_param = sort_list->cmp_param;
	callback_params.user_data = data;

	begin = SortLBegin(sort_list);
	end = SortLEnd(sort_list);

	/* find an element which its data is bigger than the data provided by the user. */
	return_itr = SortLFindIf(begin, end, is_bigger_p, &callback_params);

	/* insert new node in the returned location */
	return_itr.dlist_itr = DListInsert(return_itr.dlist_itr, data);

	return return_itr;
}


/*******************************************************************************
***************************** SortL GetData ***********************************/
void *SortLGetData(sortl_itr_ty iter)
{
	return DListGetData(iter.dlist_itr);
}


/*******************************************************************************
***************************** SortL Destroy ***********************************/
void SortLDestroy(sortl_ty *sort_list)
{
	ASSERT_NOT_NULL_IMP(sort_list);

	/* free dlist with DListDestroy */
	DListDestroy(sort_list->dlist);

	/* break sortl_ty fields */
    DEBUG_MODE
    (
    	sort_list->dlist = INVALID_PTR;
    	sort_list->cmp_param = INVALID_PTR;
    )
	free(sort_list);
}

/*******************************************************************************
***************************** SortL Count *************************************/
size_t SortLCount(const sortl_ty *sort_list)
{
	ASSERT_NOT_NULL_IMP(sort_list);

	return DListCount(sort_list->dlist);
}

/*******************************************************************************
***************************** SortL Begin *************************************/
sortl_itr_ty SortLBegin(sortl_ty *sort_list)
{
	sortl_itr_ty begin = {NULL};

	ASSERT_NOT_NULL_IMP(sort_list);

	begin.dlist_itr = DListBegin(sort_list->dlist);

	return begin;
}


/*******************************************************************************
***************************** SortL End ***************************************/

sortl_itr_ty SortLEnd(sortl_ty *sort_list)
{
	sortl_itr_ty end = {NULL};

	ASSERT_NOT_NULL_IMP(sort_list);

	end.dlist_itr = DListEnd(sort_list->dlist);

	return end;
}


/*******************************************************************************
***************************** SortL Next **************************************/
sortl_itr_ty SortLNext(sortl_itr_ty iter)
{
	sortl_itr_ty next = {NULL};

	next.dlist_itr = DListNext(iter.dlist_itr);

	return next;
}

/*******************************************************************************
***************************** SortL Prev **************************************/
sortl_itr_ty SortLPrev(sortl_itr_ty iter)
{
	sortl_itr_ty prev = {NULL};

	prev.dlist_itr = DListPrev(iter.dlist_itr);

	return prev;
}

/*******************************************************************************
***************************** SortL IsSameIter ********************************/
int SortLIsSameIter(sortl_itr_ty iter1, sortl_itr_ty iter2)
{
	return DListIsSameIter(iter1.dlist_itr, iter2.dlist_itr);
}


/*******************************************************************************
***************************** SortL Merge *************************************/
void SortLMerge(sortl_ty *dest, sortl_ty *donor)
{
	callback_params_sl_ty callb_params = {NULL};

	sortl_itr_ty dest_where = {NULL};
	sortl_itr_ty donor_from = {NULL};
	sortl_itr_ty donor_to = {NULL};

	ASSERT_NOT_NULL_IMP(dest);
	ASSERT_NOT_NULL_IMP(donor);

	dest_where = SortLBegin(dest);
	donor_from = SortLBegin(donor);

	/* fill cmp_objects_package fields with dest's comparison information */
	callb_params.cmp_func_p = dest->p_cmp_func;
	callb_params.cmp_param = dest->cmp_param;
	callb_params.user_data = SortLGetData(donor_from);

	/* traverse dest list until the end */
	while(!SortLIsEmpty(donor))
	{
		/* in dest traverse 'where' until is bigger than 'from' donor element */
		dest_where = SortLFindIf(dest_where, SortLEnd(dest), IsBiggerImp, &callb_params);
		/* change param comparison to dest_where value */
		callb_params.user_data = SortLGetData(dest_where);

		/* In case where got the the end of dest, the rest of donor will be copied to dest */
		if (SortLIsSameIter(dest_where, SortLEnd(dest)))
		{
			donor_to = SortLEnd(donor);
		}
		else
		{
			/* in donor traverse 'to' until is bigger than 'where' dest element */
			donor_to = SortLFindIf(donor_from, SortLEnd(donor), IsBiggerImp, &callb_params);
			/* change param comparison to donor_to value */
			callb_params.user_data = SortLGetData(donor_to);
		}

		/* copy and remove range of donor elements to dest list */
		DListSplice(dest_where.dlist_itr, donor_from.dlist_itr, donor_to.dlist_itr);

		donor_from = donor_to;
	}
}

/* PsuedoCode
		where							end
dest 	1	--	12	--	80	--	120	--	END
donor	2	--	3	--	22	--	36	--	77	--	600	--	END
		from-to											end
*/

/*******************************************************************************
***************************** SortL Find **************************************/
sortl_itr_ty SortLFind(const sortl_ty *sortl, const void *data)
{
	callback_params_sl_ty params = {NULL};
	IsMatchFunc is_equal_p = IsEqualImp;
	sortl_itr_ty ret_itr = {NULL};

	ASSERT_NOT_NULL_IMP(sortl);

	/* fill callback_params_sl_ty fields with comparison information */
	params.cmp_func_p = sortl->p_cmp_func;
	params.cmp_param = sortl->cmp_param;
	params.user_data = (void *)data;

	/* find an element which its data equals the data provided by the user */
	ret_itr.dlist_itr = DListFind(DListBegin(sortl->dlist), DListEnd(sortl->dlist),
								is_equal_p, &params);

	return ret_itr;
}

/*******************************************************************************
***************************** SortL IsEmpty ***********************************/

int SortLIsEmpty(const sortl_ty *sort_list)
{
	ASSERT_NOT_NULL_IMP(sort_list);

	return DListIsEmpty(sort_list->dlist);
}


/*******************************************************************************
***************************** SortL Remove ************************************/

sortl_itr_ty SortLRemove(sortl_itr_ty iter)
{
	sortl_itr_ty ret_itr = {NULL};

	ret_itr.dlist_itr = DListRemove(iter.dlist_itr);

	return ret_itr;
}


/*******************************************************************************
***************************** SortL FindIf ************************************/
sortl_itr_ty SortLFindIf(sortl_itr_ty from, sortl_itr_ty to, IsMatchFunc is_match_func, void *param)
{
	sortl_itr_ty ret_itr = {NULL};

	assert (from.dlist_itr.dlist == to.dlist_itr.dlist
	&& "FindIf: Iterators refer to the same list");
	assert (NULL != is_match_func
	&& "FindIf: Function pointer is invalid");

	/* find matched element; in case not found get the end of range */
	ret_itr.dlist_itr = DListFind(from.dlist_itr, to.dlist_itr, is_match_func, param);

	return ret_itr;
}


/*******************************************************************************
***************************** Side Functions **********************************/
int IsBiggerImp(const void *element_data, const void *param)
{
	int is_bigger = 0;

	/* convert 'param' to callback_params_sl_ty data type */
	callback_params_sl_ty callb_params = *(callback_params_sl_ty *)param;

	/* determine if data's node element is bigger than param value */
	is_bigger = callb_params.cmp_func_p(element_data, callb_params.user_data,
										callb_params.cmp_param);

	return (is_bigger > 0);
}

int IsEqualImp(const void *element_data, const void *param)
{
	int is_equal = 0;

	/* convert 'param' to callback_params_sl_ty data type */
	callback_params_sl_ty callb_params = *(callback_params_sl_ty *)param;

	/* determine if data's node element equals to param value */
	is_equal = callb_params.cmp_func_p(element_data, callb_params.user_data,
										callb_params.cmp_param);

	return (is_equal == 0);
}




