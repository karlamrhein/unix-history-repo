/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fnmatch.c	5.5 (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

/*
 * Function fnmatch() as proposed in Posix 1003.2 B.6 (rev. 9).
 * Compares a filename or pathname to a pattern.
 */

#include <fnmatch.h>
#include <string.h>

#define	EOS	'\0'

static char *rangematch __P((char *, int));

fnmatch(pattern, string, flags)
	register const char *pattern, *string;
	int flags;
{
	register char c;
	char test;

	for (;;)
		switch (c = *pattern++) {
		case EOS:
			return (*string == EOS ? 0 : FNM_NOMATCH);
		case '?':
			if ((test = *string++) == EOS ||
			    test == '/' && flags & FNM_PATHNAME)
				return (FNM_NOMATCH);
			break;
		case '*':
			c = *pattern;
			/* Collapse multiple stars. */
			while (c == '*')
				c = *++pattern;

			/* Optimize for pattern with * at end or before /. */
			if (c == EOS)
				if (flags & FNM_PATHNAME)
					return (index(string, '/') == NULL ?
					    0 : FNM_NOMATCH);
				else
					return (0);
			else if (c == '/' && flags & FNM_PATHNAME) {
				if ((string = index(string, '/')) == NULL)
					return (FNM_NOMATCH);
				break;
			}

			/* General case, use recursion. */
			while ((test = *string) != EOS) {
				if (!fnmatch(pattern, string, flags))
					return (0);
				if (test == '/' && flags & FNM_PATHNAME)
					break;
				++string;
			}
			return (FNM_NOMATCH);
		case '[':
			if ((test = *string++) == EOS ||
			    test == '/' && flags & FNM_PATHNAME)
				return (FNM_NOMATCH);
			if ((pattern = rangematch(pattern, test)) == NULL)
				return (FNM_NOMATCH);
			break;
		case '\\':
			if (!(flags & FNM_NOESCAPE)) {
				if ((c = *pattern++) == EOS) {
					c = '\\';
					--pattern;
				}
				if (c != *string++)
					return (FNM_NOMATCH);
				break;
			}
			/* FALLTHROUGH */
		default:
			if (c != *string++)
				return (FNM_NOMATCH);
			break;
		}
	/* NOTREACHED */
}

static char *
rangematch(pattern, test)
	register char *pattern;
	register int test;
{
	register char c, c2;
	int negate, ok;

	if (negate = (*pattern == '!'))
		++pattern;

	/*
	 * XXX
	 * TO DO: quoting
	 */
	for (ok = 0; (c = *pattern++) != ']';) {
		if (c == EOS)
			return (NULL);		/* Illegal pattern. */
		if (*pattern == '-' && (c2 = pattern[1]) != EOS && c2 != ']') {
			if (c <= test && test <= c2)
				ok = 1;
			pattern += 2;
		}
		else if (c == test)
			ok = 1;
	}
	return (ok == negate ? NULL : pattern);
}
