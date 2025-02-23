/*******************************************************************************
********************************* - UID - **************************************
*
*	DESCRIPTION		API UID
*	AUTHOR 			Liad Raz
*   FILES           uid.c, uid_test.c, uid.h
*
*******************************************************************************/

#ifndef __UID_H__
#define __UID_H__

#include <sys/types.h>	/* pid_t, time_t, size_t */
#include <unistd.h>	    /* getpid */
#include <time.h>	    /* time() */

typedef struct uid uid_ty;

static const uid_ty BAD_UID;

/*******************************************************************************
* DESCRIPTION	Creates UID handle.

* Time Complexity   O(1)
*******************************************************************************/
uid_ty UIDGenerate(void);


/*******************************************************************************
* DESCRIPTION	Evalutes if two UID's are equal
* RETURN		boolean => 	1 SAME; 0 DIFFERENT.

* Time Complexity   O(1)
*******************************************************************************/
int UIDIsSame(uid_ty uid1, uid_ty uid2);


/*******************************************************************************
* DESCRIPTION	Evalutes if UID is invalid
* RETURN		boolean => 	1 is BAD; 0 NOT.

* Time Complexity   O(1)
*******************************************************************************/
int UIDIsBad(const uid_ty uid);


struct uid
{
    size_t counter;
    time_t time;
    pid_t pid;
};

#endif /* __UID_H__ */

