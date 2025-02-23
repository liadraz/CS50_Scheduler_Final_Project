/*******************************************************************************
**************************** - DLINKED_LIST - **********************************
*
*	DESCRIPTION		Test of Doubly Linked List
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdio.h>		/* printf, puts */
#include <stddef.h>		/* size_t */

#include "utilities.h"
#include "dlinked_list.h"

void TestDListCreate(void);
void TestDListIsEmpty(void);
void TestDListBeginEnd(void);
void TesrDListIsSameIter(void);

void TestDListInsert(void);
void TestDListRemove(void);
void TestDListCount(void);

void TestDListForEach(void);
void TestDListSplice(void);

void TestDListPushBack(void);
void TestDListPushFront(void);
void TestDListPopBack(void);
void TestDListPopFront(void);

static void PrintDListStr(dlist_ty *dlist);
static void PrintDListInt(dlist_ty *dlist);
static int MultipleDataAndParam(void *data, void *param);

int main(void)
{
	puts("\n\t~~~~~~~~ DS - DOUBLE LINKED LIST ~~~~~~~~");

	TestDListCreate();
	TestDListIsEmpty();
	TestDListBeginEnd();
	TesrDListIsSameIter();

	TestDListInsert();
	TestDListRemove();
	TestDListCount();

	TestDListForEach();
	TestDListSplice();

	TestDListPushBack();
	TestDListPushFront();
	TestDListPopBack();
	TestDListPopFront();

	UNUSED(PrintDListInt);

	return 0;
}


void TestDListCreate(void)
{
	dlist_ty *dlist = DListCreate();

	PRINT_MSG(\n--- Test Init and Destroy dlist ---);
	puts("==> Create");
	PRINT_PTR_ADDRESS(dlist);

	if (NULL != dlist)
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
	DListDestroy(dlist);
}


void TestDListIsEmpty(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = DListBegin(dlist);
	int num = 120;

	PRINT_MSG(\n------------------);

	puts("==> IsEmpty");
	PRINT_MSG(dlist has no elements);
	PRINT_IS_SUCCESS(DListIsEmpty(dlist), empty);

	itr = DListInsert(itr, (void *)&num);

	PRINT_MSG(dlist has Two Elements);
	PRINT_IS_SUCCESS(DListIsEmpty(dlist), empty);

	DListDestroy(dlist);
}


void TestDListBeginEnd(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr_begin = DListBegin(dlist);
	dlist_itr_ty itr_end = DListEnd(dlist);

	PRINT_MSG(\n--- Test Begin and End when dlist is empty ---);
	puts("==> Begin");
	PRINT_PTR_ADDRESS(itr_begin.to_node);
	if (itr_begin.to_node == itr_end.to_node)
	{
		GREEN;
		PRINT_MSG(\tbegin SUCCESS);
		DEFAULT;
	}

	puts("==> End");
	PRINT_PTR_ADDRESS(itr_end.to_node);
	if (itr_begin.to_node == itr_end.to_node)
	{
		GREEN;
		PRINT_MSG(\tend SUCCESS);
		DEFAULT;
	}

	DListDestroy(dlist);
}

void TesrDListIsSameIter(void)
{
	dlist_ty *dlist1 = DListCreate();
	dlist_itr_ty itr_begin = DListBegin(dlist1);

	dlist_ty *dlist2 = DListCreate();
	dlist_itr_ty itr_end = DListEnd(dlist2);

	int is_same_itr = 0;

	PRINT_MSG(\n--- Test Is Same ---);

	puts("==> IsSame");
	is_same_itr = DListIsSameIter(itr_begin, itr_end);

	PRINT_MSG(\tDifferent Lists);
	PRINT_IS_SUCCESS(is_same_itr, same);

	is_same_itr = DListIsSameIter(itr_begin, DListEnd(dlist1));
	PRINT_MSG(\tSame Lists);
	PRINT_IS_SUCCESS(is_same_itr, same);

	DListDestroy(dlist1);
	DListDestroy(dlist2);
}


void TestDListInsert(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = DListBegin(dlist);

	char *str1 = "THIRD\nAGE = 34\nName: Liad Raz\n";
	char *str2 = "SECOND\nAGE = 24\nName: Moria Gudiner\n";
	char *str3 = "FIRST\nAGE = 21\nName: Amram Even Tzur\n";

	itr = DListInsert(itr, str1);
	itr = DListInsert(itr, str2);
	itr = DListInsert(itr, str3);

	PrintDListStr(dlist);

	if (DListGetData(itr) == str3)
	{
		PRINT_MSG(\n--- Test Insert ---);

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

	DListDestroy(dlist);
}

void TestDListRemove(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = DListBegin(dlist);

	char *str1 = "THIRD\nAGE = 34\nName: Liad Raz\n";
	char *str2 = "SECOND\nAGE = 24\nName: Moria Gudiner\n";
	char *str3 = "FIRST\nAGE = 21\nName: Amram Even Tzur\n";

	itr = DListInsert(itr, str1);
	itr = DListInsert(itr, str2);
	itr = DListInsert(itr, str3);

	itr = DListRemove(itr);

	if (DListGetData(itr) == str2)
	{
		PRINT_MSG(\n--- Test Remove ---);
		PrintDListStr(dlist);

		GREEN;
		PRINT_MSG(\tRemove SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tRemove FAILED);
		DEFAULT;
	}

	DListDestroy(dlist);
}

void TestDListCount(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = DListBegin(dlist);
	size_t counter = 0;

	char *str1 = "THIRD\nAGE = 34\nName: Liad Raz\n";
	char *str2 = "SECOND\nAGE = 24\nName: Moria Gudiner\n";
	char *str3 = "FIRST\nAGE = 21\nName: Amram Even Tzur\n";

	itr = DListInsert(itr, str1);
	itr = DListInsert(itr, str2);
	itr = DListRemove(itr);
	itr = DListInsert(itr, str3);
	itr = DListInsert(itr, str3);

	itr = DListRemove(itr);

	counter = DListCount(dlist);

	if (counter == 2)
	{
		PRINT_MSG(\n--- Test Count ---);
		PRINTM_SIZE_T(result, counter);

		GREEN;
		PRINT_MSG(\tCount SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tCount FAILED);
		DEFAULT;
	}

	DListDestroy(dlist);
}

void TestDListForEach(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty end_itr = DListEnd(dlist);
	dlist_itr_ty itr = {NULL};
	int ret_value = 0;

	int num1 = 5;
	int num2 = 10;
	int num3 = 15;
	int *param = &num1;

	itr = DListInsert(DListBegin(dlist), &num1);
	itr = DListInsert(itr, &num2);
	itr = DListInsert(itr, &num3);

	ret_value = DListForEach(itr, end_itr, MultipleDataAndParam, param);

	PRINT_MSG(\n--- Test ForEach ---);
	if ((ret_value == 0) && *(int *)DListGetData(itr) == 75)
	{
		PRINTM_INT(Status, ret_value);
		GREEN;
		PRINT_MSG(\tFor Each SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tFor Each FAILED);
		DEFAULT;
	}
	DListDestroy(dlist);
}

void TestDListSplice(void)
{
	dlist_ty *target = DListCreate();
	dlist_ty *src = DListCreate();

	dlist_itr_ty itr_target = {NULL};
	dlist_itr_ty itr_src = {NULL};
	dlist_itr_ty to = {NULL};

	int num4 = 4;
	int num3 = 3;
	int num2 = 2;
	int num1 = 1;

	itr_target = DListInsert(DListBegin(target), &num3);
	itr_target = DListInsert(itr_target, &num2);
	itr_target = DListInsert(itr_target, &num1);

	itr_src = DListInsert(DListBegin(src), &num4);
	itr_src = DListInsert(itr_src, &num3);
	itr_src = DListInsert(itr_src, &num2);
	itr_src = DListInsert(itr_src, &num1);

	to = DListNext(DListNext(DListNext(DListBegin(src))));

	DListSplice(DListNext(DListBegin(target)), DListBegin(src), to);

	if (6 == DListCount(target) && 1 == DListCount(src))
	{
		GREEN;
		PRINT_STATUS_MSG(Test DListSplice: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tTest DListSplice: FAILED);
		DEFAULT;
	}

	DListDestroy(target);
}


void TestDListPushBack(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = {NULL};

	int num1 = 5;
	int num2 = 10;
	int num3 = 15;
	int num4 = 888;
	int status = -1;

	itr = DListInsert(DListBegin(dlist), &num1);
	itr = DListInsert(itr, &num2);
	itr = DListInsert(itr, &num3);

	status = DListPushBack(dlist, &num4);
	status = DListPushBack(dlist, &num4);
	status = DListPushBack(dlist, &num4);

	if (!status)
	{
		GREEN;
		PRINT_STATUS_MSG(Test PushBack: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tTest PushBack: FAILED);
		DEFAULT;
	}

	DListDestroy(dlist);
}

void TestDListPushFront(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = {NULL};

	int num1 = 5;
	int num2 = 10;
	int num3 = 15;
	int num4 = 888;
	int status = -1;

	itr = DListInsert(DListBegin(dlist), &num1);
	itr = DListInsert(itr, &num2);
	itr = DListInsert(itr, &num3);

	status = DListPushFront(dlist, &num4);

	if (0 == status)
	{
		GREEN;
		PRINT_STATUS_MSG(Test PushFront: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tTest PushFront: FAILED);
		DEFAULT;
	}

	DListDestroy(dlist);
}

void TestDListPopBack(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = {NULL};

	int num1 = 5;
	int num2 = 10;
	int num3 = 15;

	itr = DListInsert(DListBegin(dlist), &num1);
	itr = DListInsert(itr, &num2);
	itr = DListInsert(itr, &num3);

	DListPopBack(dlist);

	if (2 == DListCount(dlist))
	{
		GREEN;
		PRINT_STATUS_MSG(Test PopBack: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tTest PopBack: FAILED);
		DEFAULT;
	}

	DListDestroy(dlist);
}

void TestDListPopFront(void)
{
	dlist_ty *dlist = DListCreate();
	dlist_itr_ty itr = {NULL};

	int num1 = 5;
	int num2 = 10;
	int num3 = 15;

	itr = DListInsert(DListBegin(dlist), &num1);
	itr = DListInsert(itr, &num2);
	itr = DListInsert(itr, &num3);

	DListPopFront(dlist);

	if (2 == DListCount(dlist))
	{
		GREEN;
		PRINT_STATUS_MSG(Test PopFront: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_MSG(\tTest PopFront: FAILED);
		DEFAULT;
	}

	DListDestroy(dlist);
}

/******************************************************************************/
/******************************************************************************/

static int MultipleDataAndParam(void *num1, void *num2)
{
	*(int *)num1 *= *(int *)num2;

	return 0;
}

static void PrintDListStr(dlist_ty *dlist)
{
	dlist_itr_ty tmp_itr = DListBegin(dlist);
	size_t i = 0;
	size_t len = DListCount(dlist);

	puts("------------------");
	for (;i < len; ++i)
	{
		printf("NODE (%lu)\n%s", i + 1, (char *)DListGetData(tmp_itr));
		puts("------------------");
		puts("\tV");
		puts("------------------");

		tmp_itr = DListNext(tmp_itr);
	}
}

static void PrintDListInt(dlist_ty *dlist)
{
	dlist_itr_ty tmp_itr = DListBegin(dlist);
	size_t i = 0;
	size_t len = DListCount(dlist);

	puts("----------");
	for (;i < len; ++i)
	{
		printf("%d\n", *(int *)DListGetData(tmp_itr));
		puts("----------");
		puts("VV");
		puts("----------");

		tmp_itr = DListNext(tmp_itr);
	}
}







