/*******************************************************************************
********************************* - UID - **************************************
*
*	DESCRIPTION		Tests UID
*	AUTHOR          Liad Raz
*
*******************************************************************************/


#include <stdio.h>		/* printf, puts */
#include <stddef.h>		/* size_t */

#include "utilities.h"
#include "uid.h"

void TestUIDGenerate(void);
void TestUIDIsSame(void);
void TestUIDIsBad(void);

static void TestTimeFunctions(void);
static void PrintUID(uid_ty uid);

int main(void)
{
	puts("\n\t~~~~~~~~ DS - Generator ~~~~~~~~");
	TestUIDGenerate();
	TestUIDIsSame();
	TestUIDIsBad();

	TestTimeFunctions();

	return 0;
}


void TestUIDGenerate(void)
{
	uid_ty uid1 = UIDGenerate();
	uid_ty uid2 = UIDGenerate();
	uid_ty uid3 = UIDGenerate();
	uid_ty uid4 = UIDGenerate();

	PrintUID(uid1);
	PrintUID(uid2);
	PrintUID(uid3);
	PrintUID(uid4);
}

void TestUIDIsSame(void)
{
	uid_ty uid1 = UIDGenerate();
	uid_ty uid2 = UIDGenerate();

	puts("==> IsSame");
	if (UIDIsSame(uid1, uid2))
	{
		RED;
		PRINT_MSG(\tUIDs are the SAME);
		DEFAULT;
	}
	else
	{
		GREEN;
		PRINT_MSG(\tUIDs are NOT SAME);
		DEFAULT;
	}
}

void TestUIDIsBad(void)
{
	int is_bad = 0;
	uid_ty uid1 = UIDGenerate();
	puts("\n==> (Bad_UID == Bad_UID)");
	is_bad = UIDIsBad(BAD_UID);
	PRINT_IS_SUCCESS(is_bad, BAD_UID);

	puts("==> (uid1 == Bad_UID)");
	is_bad = UIDIsBad(uid1);
	PRINT_IS_SUCCESS(is_bad, BAD_UID);
}


static void TestTimeFunctions(void)
{
	time_t seconds = time(NULL);

	printf("Current Time in seconds %lu\n", seconds);
	printf("Current Time %s\n", ctime(&seconds));
}

static void PrintUID(uid_ty uid)
{
	puts("=> new UID");
	printf("Number:\t%lu\n", uid.counter);
	printf("Time:\t%lu\n", uid.time);
	printf("PID:\t%d\n", uid.pid);
	puts("\n");
}
