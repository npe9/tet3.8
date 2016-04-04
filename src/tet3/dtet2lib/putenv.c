/*
 *      SCCS:  @(#)putenv.c	1.11 (11/03/17) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992,2011 The Open Group
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * The Open Group, Boundaryless Information Flow and the ``X Device'' are
 * trademarks and UNIX is a registered trademark of The Open Group in the
 * United States and other countries.
 */

#ifndef lint
static char sccsid[] = "@(#)putenv.c	1.11 (11/03/17) TETware release 3.8a";
#endif

/************************************************************************

SCCS:   	@(#)putenv.c	1.11 11/03/17 TETware release 3.8a
NAME:		putenv.c
PRODUCT:	TETware
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	environment manipulation function

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., February 1993
	allow user to modify environment between calls

	Andrew Dingwall, UniSoft Ltd., July 1997
	added support the MT DLL version of the C runtime support library
	on Win32 systems

	Andrew Dingwall, UniSoft Ltd., July 1999
	added support for shared API libraries

	Geoff Clare, The Open Group, March 2011
	Use putenv() or setenv() if available.  If not, don't change
	environ pointers without changing environ itself.

************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#ifndef _WIN32	/* -WIN32-CUT-LINE- */
#  include <string.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif		/* -WIN32-CUT-LINE- */
#include "dtmac.h"
#include "dtetlib.h"
#include "error.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	tet_putenv() - add an environment string to the environment
**
**	return 0 if successful or -1 on error
**
**	this routine is here because not all systems have putenv(3)
*/

TET_IMPORT int tet_putenv(s)
char *s;
{
#ifdef _WIN32	/* -START-WIN32-CUT- */

	if (_putenv(s) < 0) {
		error(errno, "_putenv() failed", (char *) 0);
		return(-1);
	}

	return(0);

#else		/* -END-WIN32-CUT- */

	/*
	 * Decide whether to use putenv(), setenv(), or environ:
	 *
	 *   By default if putenv() is available, it will be used.
	 *   Otherwise if setenv() is available, it will be used.
	 *   Otherwise a compiler error message will be produced
	 *   asking for an explicit choice.
	 *
	 *   Direct modification of environ is only used if
	 *   explicitly requested, as the other methods are much
	 *   preferred.
	 *
	 * An explicit choice can be made by defining TET_USE_PUTENV,
	 * TET_USE_SETENV, or TET_USE_ENVIRON using a -D compiler
	 * flag in the DTET_CDEFS variable in defines.mk.
	 */
#  if !defined(TET_USE_PUTENV) && !defined(TET_USE_SETENV) && \
      !defined(TET_USE_ENVIRON) 
#    if defined(_XOPEN_SOURCE) || defined(_XOPEN_VERSION) 
#      define TET_USE_PUTENV /* putenv() is in all the X/Open specs */
#    elif _POSIX_C_SOURCE >= 200112 || _POSIX_VERSION >= 200112
#      define TET_USE_SETENV /* setenv() is in POSIX.1-2001 and later */
#    endif
#  endif /* !defined(TET_USE_PUTENV) && !defined(TET_USE_SETENV) && !defined(TET_USE_ENVIRON) */

#  if defined(TET_USE_PUTENV)

	/* Use putenv() */

#    if !defined(_XOPEN_SOURCE)
	/* visibility was not requested, so it might not have been declared */
#      undef putenv
	extern int putenv();
#    endif

	if (putenv(s) != 0)
	{
		error(errno, "putenv() failed", (char *) 0);
		return -1;
	}

	return 0;

#  elif defined(TET_USE_SETENV)

	/* Use setenv() */

#    if !(_POSIX_C_SOURCE >= 200112 || _XOPEN_SOURCE+0 >= 600)
	/* visibility was not requested, so it might not have been declared */
#      undef setenv
	extern int setenv();
#    endif

	char *envname;
	size_t n;

	n = strcspn(s, "=");
	envname = malloc(n+1);
	if (envname == NULL)
	{
		error(errno, "can't allocate envname", (char *) 0);
		return -1;
	}
	(void) strncpy(envname, s, n);
	envname[n] = '\0';

	if (setenv(envname, &s[n+1], 1) == -1)
	{
		error(errno, "setenv() failed", (char *) 0);
		free(envname);
		return -1;
	}

	free(envname);
	return 0;

#  elif defined(TET_USE_ENVIRON)

	/* Modify environ directly */

	/*
	 * The method used here always creates a new environment list,
	 * even if the variable already exists, because modifying an
	 * environ pointer without changing environ itself is unsafe.
	 * Implementations can maintain internal state (e.g. to make
	 * searching efficient) which will become out of sync if
	 * an environ pointer is modified, unless environ itself
	 * changes in which case the implementation will notice this
	 * and recreate the internal state.
	 */

	static char **oldenv;
	char **newenv, **ep1, **ep2;
	char *p1, *p2;
	extern char **environ;

	/* find the end of the environ list */
	for (ep1 = environ; *ep1; ep1++)
		/* nothing */ ;

	/* 
	 * allocate a new environment space, using malloc (not
	 * realloc or BUFCHK, since they might not move it).
	 */
	newenv = malloc(((ep1 - environ) + 2) * sizeof *newenv);
	if (newenv == NULL)
	{
		error(errno, "can't allocate new environ", (char *) 0);
		return -1;
	}

	/* copy in the old environment and remember the end in ep2 */
	for (ep1 = environ, ep2 = newenv; *ep1; ep1++, ep2++)
		*ep2 = *ep1;

	/* if the variable is already there, update its value */
	for (ep1 = newenv; *ep1; ep1++)
	{
		for (p1 = *ep1, p2 = s; *p1 && *p2; p1++, p2++)
		{
			if (*p1 != *p2 || *p1 == '=')
				break;
		}
		if (*p1 == '=' && *p2 == '=')
		{
			*ep1 = s;
			break;
		}
	}

	/* if not there, add it to the end */
	if (*ep1 == NULL)
		*ep2++ = s;

	/* null terminate the new list */
	*ep2 = NULL;

	/* switch the environments and free the last one we allocated */
	environ = newenv;
	if (oldenv != NULL)
		free(oldenv);
	oldenv = environ;

	return 0;

#  else /* TET_USE_* */

	/* Don't know what to use */

#    error Please define TET_USE_PUTENV, TET_USE_GETENV, or TET_USE_ENVIRON \
           See comments in src/tet3/dtet2lib/putenv.c

	return -1;

#  endif /* TET_USE_* */

#endif		/* -WIN32-CUT-LINE- */

}

