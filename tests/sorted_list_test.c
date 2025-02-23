/*******************************************************************************
****************************** - SORTED_LIST - ********************************
*
*	DESCRIPTION		Test File - Sorted list
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdio.h>		/* printf, puts */
#include <stddef.h>		/* size_t */

#include "utilities.h"
#include "sorted_list.h"

void TestSortLCreate(void);
void TestSortLInsert(void);
void TestSortLGetData(void);
void TestSortLCount(void);
void TestSortLBeginEnd(void);
void TestSortLNextPrev(void);
void TestSortLIsSameIter(void);
void TestSortLFind(void);
void TestSortLMerge(void);

static int CmpObjects(const void *obj1, const void *obj2, const void *key);
static void PrintSortedList(sortl_ty *sort_list);


int main(void)
{
	puts("\n\t~~~~~~~~ DS - SORTED LINKED LIST ~~~~~~~~");

	TestSortLCreate();
	TestSortLInsert();
	TestSortLCount();
	TestSortLBeginEnd();
	TestSortLNextPrev();
	TestSortLIsSameIter();
	TestSortLFind();
	TestSortLMerge();

	return 0;
}


void TestSortLCreate(void)
{
	int key = 10;
	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);

	PRINT_MSG(\n--- Test Create sort list ---);
	puts("==> Create");
	PRINT_PTR_ADDRESS(sort_list);

	if (NULL != sort_list)
	{
		GREEN;
		PRINT_STATUS_MSG(Create SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Create FAILED);
		DEFAULT;
	}

	puts("==> Destroy");
	SortLDestroy(sort_list);
}

void TestSortLInsert(void)
{
	int key = 1;
	int num1 = 220;
	int num2 = 80;
	int num3 = 770;
	int num4 = 5;

	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);

	PRINT_MSG(\n--- Test Insert sort list ---);

	SortLInsert(sort_list, (void *)&num1);
	SortLInsert(sort_list, (void *)&num2);
	SortLInsert(sort_list, (void *)&num3);
	SortLInsert(sort_list, (void *)&num4);

	if (*(int *)(SortLGetData(SortLBegin(sort_list))) == num4)
	{
		PRINT_MSG(\n--- Test Insert ---);
		PrintSortedList(sort_list);

		GREEN;
		PRINT_MSG(\tInsert SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tInsert FAILED);
		DEFAULT;
	}

	SortLDestroy(sort_list);
}


void TestSortLCount(void)
{
	int key = 1;
	int num1 = 220;
	int num2 = 100;
	int num3 = 5;
	size_t counter = 0;
	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);

	PRINT_MSG(\n--- Test Count sort list ---);

	SortLInsert(sort_list, (void *)&num1);
	SortLInsert(sort_list, (void *)&num2);
	SortLInsert(sort_list, (void *)&num3);
	SortLInsert(sort_list, (void *)&num3);
	SortLInsert(sort_list, (void *)&num3);
	SortLInsert(sort_list, (void *)&num3);

	counter = SortLCount(sort_list);

	if (counter == 6)
	{
		puts("==> Inserted 6 Nodes");

		GREEN;
		PRINTM_SIZE_T(RESULT, counter);
		PRINT_MSG(\tCount SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tCount FAILED);
		DEFAULT;
	}

	SortLDestroy(sort_list);
}


void TestSortLBeginEnd(void)
{
	int key = 1;
	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);

	sortl_itr_ty itr_begin = SortLBegin(sort_list);
	sortl_itr_ty itr_end = SortLEnd(sort_list);

	PRINT_MSG(\n--- Test Begin and End when sorted list is empty ---);

	puts("==> Begin");
	PRINT_PTR_ADDRESS(itr_begin.dlist_itr.to_node);
	if (itr_begin.dlist_itr.to_node == itr_end.dlist_itr.to_node)
	{
		GREEN;
		PRINT_MSG(\tbegin SUCCESS);
		DEFAULT;
	}

	puts("==> End");
	PRINT_PTR_ADDRESS(itr_begin.dlist_itr.to_node);
	if (itr_begin.dlist_itr.to_node == itr_end.dlist_itr.to_node)
	{
		GREEN;
		PRINT_MSG(\tend SUCCESS);
		DEFAULT;
	}

	SortLDestroy(sort_list);
}

void TestSortLNextPrev(void)
{
	int key = 1;
	int num10 = 10;
	int num20 = 20;

	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);
	sortl_itr_ty add_itr10 = SortLInsert(sort_list, (void *)&num10);
	sortl_itr_ty add_itr20 = SortLInsert(sort_list, (void *)&num20);
	sortl_itr_ty add_itr10Prev = add_itr10;

	PRINT_MSG(\n--- Test Next and Prev ---);

	puts("==> Next");
	add_itr10 = SortLNext(add_itr10);
	if (add_itr10.dlist_itr.to_node == add_itr20.dlist_itr.to_node)
	{
		GREEN;
		PRINT_MSG(\tNext SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tNext FAILED);
		DEFAULT;
	}

	puts("==> Prev");
	add_itr10 = SortLPrev(add_itr10);
	if (add_itr10.dlist_itr.to_node == add_itr10Prev.dlist_itr.to_node)
	{
		GREEN;
		PRINT_MSG(\tPrev SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tPrev FAILED);
		DEFAULT;
	}

	SortLDestroy(sort_list);
}


void TestSortLIsSameIter(void)
{
	int key = 1;

	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);
	sortl_itr_ty begin = SortLBegin(sort_list);
	sortl_itr_ty end = SortLEnd(sort_list);

	PRINT_MSG(\n--- Test IsSame ---);

	puts("==> IsSame");
	if (SortLIsSameIter(begin, end))
	{
		GREEN;
		PRINT_MSG(\tIsSame SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tIsSame FAILED);
		DEFAULT;
	}

	SortLDestroy(sort_list);
}

void TestSortLFind(void)
{
	int key = 1;
	int num1 = 10;
	int num2 = 20;
	int num3 = 5;
	int data_to_find = 5;
	sortl_itr_ty found_itr = {NULL};
	sortl_itr_ty num3_itr = {NULL};
	sortl_ty *sort_list = SortLCreate(CmpObjects, (void *)&key);

	SortLInsert(sort_list, (void *)&num1);
	SortLInsert(sort_list, (void *)&num2);
	num3_itr = SortLInsert(sort_list, (void *)&num3);

	PRINT_MSG(\n--- Test Find ---);

	found_itr = SortLFind(sort_list, (void *)&data_to_find);

	puts("==> Find");
	if (SortLGetData(found_itr) == SortLGetData(num3_itr))
	{
		GREEN;
		PRINT_MSG(\tFind SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tFind FAILED);
		DEFAULT;
	}

	SortLDestroy(sort_list);
}

void TestSortLMerge(void)
{
	int key = 1;
	int num1 = 80;
	int num2 = 22;
	int num3 = 44;
	int num4 = 66;

	sortl_ty *dest = SortLCreate(CmpObjects, (void *)&key);
	sortl_ty *donor = SortLCreate(CmpObjects, (void *)&key);

	SortLInsert(dest, (void *)&num1);
	SortLInsert(dest, (void *)&num2);

	SortLInsert(donor, (void *)&num3);
	SortLInsert(donor, (void *)&num4);

	PRINT_MSG(\n--- Test Merge ---);

	SortLMerge(dest, donor);

	puts("==> dest");
	PrintSortedList(dest);
	puts("==> donor");
	PrintSortedList(donor);

	SortLDestroy(dest);
	SortLDestroy(donor);
}


/*******************************************************************************
*******************************************************************************/

static void PrintSortedList(sortl_ty *sort_list)
{
	sortl_itr_ty running_itr = SortLBegin(sort_list);
	size_t i = 0;
	size_t len = SortLCount(sort_list);

	puts("------------------");
	for (;i < len; ++i)
	{
		printf("node.%lu\n\t%d\n", i + 1, *(int *)SortLGetData(running_itr));
		puts("------------------");
		puts("\t\\/ /\\");
		puts("------------------");

		running_itr = SortLNext(running_itr);
	}
}

static int CmpObjects(const void *obj1, const void *obj2, const void *key)
{
	UNUSED(key);
	return (*(int *)obj1 - *(int *)obj2);
}
