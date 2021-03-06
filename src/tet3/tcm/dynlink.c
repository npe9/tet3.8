/*
 *	SCCS: @(#)dynlink.c	1.2 (99/09/03)
 *
 *	UniSoft Ltd., London, England
 *
 * Copyright (c) 1998 The Open Group
 * All rights reserved.
 *
 * No part of this source code may be reproduced, stored in a retrieval
 * system, or transmitted, in any form or by any means, electronic,
 * mechanical, photocopying, recording or otherwise, except as stated
 * in the end-user licence agreement, without the prior permission of
 * the copyright owners.
 * A copy of the end-user licence agreement is contained in the file
 * Licence which accompanies this distribution.
 * 
 * Motif, OSF/1, UNIX and the "X" device are registered trademarks and
 * IT DialTone and The Open Group are trademarks of The Open Group in
 * the US and other countries.
 *
 * X/Open is a trademark of X/Open Company Limited in the UK and other
 * countries.
 *
 */

#ifndef lint
static char sccsid_dynlink[] = "@(#)dynlink.c	1.2 (99/09/03) TETware release 3.8";
#endif

/************************************************************************

SCCS:   	@(#)dynlink.c	1.2 99/09/03 TETware release 3.8
NAME:		dynlink.c
PRODUCT:	TETware
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	July 1998

DESCRIPTION:
	this is a simple dynamic linker for use when building a test case
	to use a shared API library on a Win32 system

	the dynamic linker is in two parts:

		tet_w32dynlink() resides in the main program
		tet_w32dlcheck() resides in the shared API library

	tet_w32dynlink() performs the dynamic linking
	tet_w32dlcheck() ensures that none of the pointers have been missed

	see the comment in dtmac.h for an overview of how this works


	no calls to TETware library functions are allowed from this file

MODIFICATIONS:

	Andrew Dingwall, UniSoft Ltd., July 1999
	moved TCM code out of the API library

************************************************************************/

/*
** This file is a component of the TCM (tcm.o) and/or one of the child
** process controllers (tcmchild.o and tcmrem.o).
** On UNIX systems, these .o files are built using ld -r.
** There is no equivalent to ld -r in MSVC, so on Win32 systems each .c
** file is #included in a scratch .c or .cpp file and a single object
** file built from that.
**
** This imposes some restictions on the contents of this file:
**
**	+ Since this file might be included in a C++ program, all
**	  functions must have both ANSI C and common C definitions.
**
**	+ The only .h file that may appear in this file is tcmhdrs.h;
**	  all other .h files that are needed must be #included in there.
**
**	+ The scope of static variables and functions encompasses all
**	  the source files, not just this file.
**	  So all static variables and functions must have unique names.
*/


/* TET_SHLIB_SOURCE implies TET_SHLIB */
#if defined(TET_SHLIB_SOURCE) && !defined(TET_SHLIB)
#  define TET_SHLIB
#endif

#if defined(_WIN32) && defined(TET_SHLIB)	/* -START-WIN32-CUT- */

#  ifdef TET_SHLIB_SOURCE
#    define TET_DLCHECK_SOURCE
     /* also makes the TET_W32DLCHECK code visible in dtmac.h */
#  else
#    define TET_DYNLINK_SOURCE
     /* also makes the TET_W32DYNLINK code visible in dtmac.h */
#  endif


   /*
   ** In Distributed TETware we need the transport-specific headers
   ** as well as the generic ones.
   ** It's OK to have this in a generic file because we only support
   ** the INET transport on Win32 systems.
   */
#  ifndef TET_LITE	/* -START-LITE-CUT- */
#    define TET_TCM_BS_C
#    define TET_TCM_IN_C
#  endif		/* -END-LITE-CUT- */

#  include "tcmhdrs.h"

#  ifdef TET_DYNLINK_SOURCE

/*
**	tet_w32dynlink() - fix up all the pointers in the shared API
**		library to point to the functions and data items
**		that have been "exported" from the program code
*/

#ifdef PROTOTYPES
void tet_w32dynlink(void)
#else
void tet_w32dynlink()
#endif
{
	/*
	** do the dynamic linking
	**
	** this file is automatically generated when the shared API library
	** is built
	*/
#	include "dynlink.gen"

	/* check that we didn't miss any of the pointers */
	tet_w32dlcheck();
}

#  endif /* TET_DYNLINK_SOURCE */

#  ifdef TET_DLCHECK_SOURCE

static int dlerrors;		/* the error counter */

/* static function declarations */
static void report_nullptr PROTOLIST((char *));

/*
**	tet_w32dlcheck() - check that all the pointers in the shared API
**		library were fixed up
**
**	there is no return on error
*/

#ifdef PROTOTYPES
TET_IMPORT void tet_w32dlcheck(void)
#else
TET_IMPORT void tet_w32dlcheck()
#endif
{
	/*
	** check all the pointers
	**
	** this file is automatically generated when the shared API library
	** is built
	*/
#       include "dlcheck.gen"

	if (dlerrors)
		exit(1);
}

/*
**	report_nullptr() - print a diagnostic if a non-fixed-up pointer
**		is found
*/

#ifdef PROTOTYPES
static void report_nullptr(char *name)
#else
static void report_nullptr(name)
char *name;
#endif
{
	(void) fprintf(stderr,
		"TCM: tet_w32dynlink() did not fix up pointer to %s\n", name);
	dlerrors++;
}


#  endif /* TET_DLCHECK_SOURCE */

#else						/* -END-WIN32-CUT- */

int tet_dynlink_c_not_used;

#endif						/* -WIN32-CUT-LINE- */

