/*
 * Copyright 1990, 1991, 1992 by the Massachusetts Institute of Technology and
 * UniSoft Group Limited.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the names of MIT and UniSoft not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  MIT and UniSoft
 * make no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * $XConsortium: strchr.c,v 1.2 92/07/01 11:59:14 rws Exp $
 */
/*
 * Emulations of strchr() and strrchr() for BSD systems without
 * them
 */

char	*index();
char	*rindex();

char *
strchr(s1, c)
char	*s1;
int 	c;
{
	return(index(s1, c));
}

char *
strrchr(s1, c)
char	*s1;
int 	c;
{
	return(rindex(s1, c));
}
