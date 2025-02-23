/*******************************************************************************
********************************* - UID - **************************************
*
*	DESCRIPTION		Implementation UID
*	AUTHOR          Liad Raz
*
*******************************************************************************/

#include "utilities.h"
#include "uid.h"


/*******************************************************************************
*********************************** Impl **************************************/
uid_ty UIDGenerate(void)
{
	uid_ty new_uid = {0};
	static size_t counter = 0;

	new_uid.counter = counter + 1;
	new_uid.time = time(NULL);
	new_uid.pid = getpid();

	return new_uid;
}

int UIDIsSame(const uid_ty uid1, const uid_ty uid2)
{
	return ((uid1.counter == uid2.counter) &&
		(uid1.time == uid2.time) &&
		(uid1.pid == uid2.pid));
}

int UIDIsBad(const uid_ty uid)
{
	return ((BAD_UID.counter == uid.counter) && (BAD_UID.time == uid.time) &&
	         (BAD_UID.pid == uid.pid));
}
