/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob.h> // Use the system's glob.h for definitions

#define Char short // Keep original character type for compatibility
#define __P(args) args

// Define missing GLOB_MAGCHAR if not present in system glob.h
#ifndef GLOB_MAGCHAR
#define GLOB_MAGCHAR 0x0100
#endif

static int glob1 __P((Char *, glob_t *));
static int glob2 __P((Char *, Char *, Char *, glob_t *));
static int glob3 __P((Char *, Char *, Char *, Char *, glob_t *));
static int globextend __P((const Char *, glob_t *));
static int match __P((Char *, Char *, Char *));
static int g_stat __P((Char *, struct stat *));
static DIR *g_opendir __P((Char *));
static Char *g_strchr __P((Char *, int));

int
glob(pattern, flags, errfunc, pglob)
	const char *pattern;
	int flags;
	int (*errfunc) __P((const char *, int));
	glob_t *pglob;
{
	const u_char *patnext;
	int c;
	Char *bufnext, *bufend, patbuf[FILENAME_MAX];

	patnext = (u_char *) pattern;
	if (flags & GLOB_APPEND) {
		pglob->gl_pathc = 0;
		pglob->gl_pathv = NULL;
		if (!(flags & GLOB_DOOFFS))
			pglob->gl_offs = 0;
	}
	
	pglob->gl_flags = flags & ~GLOB_MAGCHAR;
	pglob->gl_errfunc = errfunc;
	
	bufnext = patbuf;
	bufend = bufnext + FILENAME_MAX - 1;

	while (bufnext < bufend && (c = *patnext++) != '\0')
		*bufnext++ = c;
	*bufnext = '\0';
	
	return glob1(patbuf, pglob);
}

static int
glob1(pattern, pglob)
	Char *pattern;
	glob_t *pglob;
{
	Char *divider;

	divider = g_strchr(pattern, '/');
	if (divider == NULL)
		return glob2(pattern, NULL, NULL, pglob);

	*divider = '\0';

	if (glob2(pattern, NULL, NULL, pglob) != 0)
		return -1;

	return glob1(divider + 1, pglob);
}

static int
glob2(path, dir, pat, pglob)
	Char *path, *dir, *pat;
	glob_t *pglob;
{
	struct stat sb;
	Char *p;

	if (g_stat(path, &sb) < 0)
		return 0;

	if (!S_ISDIR(sb.st_mode))
		return 0;

	for (p = path; *p; p++);
	if (p[-1] != '/')
		*p++ = '/';
	
	if (dir)
		strcpy((char*)p, (char*)dir);

	for (p = path; *p; p++);
	
	if (pat)
		strcpy((char*)p, (char*)pat);
	
	return glob3(path, p, pat, pat, pglob);
}

static int
glob3(path, pat, rest, p, pglob)
	Char *path, *pat, *rest, *p;
	glob_t *pglob;
{
	DIR *dirp;
	struct dirent *dp;
	int err;

	if (!(dirp = g_opendir(path))) {
		if (pglob->gl_errfunc && pglob->gl_errfunc((char*)path, errno) ||
		    (pglob->gl_flags & GLOB_ERR))
			return -1;
		else
			return 0;
	}

	err = 0;
	while ((dp = readdir(dirp)) != NULL) {
		u_char *sc;
		Char *dc;

		if (dp->d_name[0] == '.' && *pat != '.')
			continue;
		
		dc = p;
		sc = (u_char *) dp->d_name;
		while (dc < rest)
			*dc++ = *sc++;
		*dc = '\0';
		
		if (match(p, pat, rest)) {
			*rest = '/';
			if (glob2(path, p, rest + 1, pglob) != 0) {
				err = -1;
				break;
			}
		}
	}
	closedir(dirp);
	return err;
}


static int
globextend(path, pglob)
	const Char *path;
	glob_t *pglob;
{
	char **pathv;
	int i;
	size_t newc, oldc;

	oldc = pglob->gl_pathc + pglob->gl_offs;
	newc = oldc + 1;
	
	pathv = (char **)realloc(pglob->gl_pathv, newc * sizeof(char *));
	if (pathv == NULL)
		return -1;
	
	pglob->gl_pathv = pathv;
	
	if (pglob->gl_offs)
		for (i = pglob->gl_offs; i >= 0; i--)
			pglob->gl_pathv[i] = pglob->gl_pathv[i-1];
	
	pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc++] = strdup((char*)path);
	return 0;
}

static int
match(name, pat, patend)
	Char *name, *pat, *patend;
{
	// Simple string comparison for this implementation
	return (strcmp((char*)name, (char*)pat) == 0);
}

static int
g_stat(fn, sb)
	Char *fn;
	struct stat *sb;
{
	char buf[FILENAME_MAX];
	Char *p;
	int i;

	for (p = fn, i = 0; *p && i < FILENAME_MAX; p++, i++)
		buf[i] = *p;
	buf[i] = '\0';
	
	return stat(buf, sb);
}

static DIR *
g_opendir(str)
	Char *str;
{
	char buf[FILENAME_MAX];
	Char *p;
	int i;

	for (p = str, i = 0; *p && i < FILENAME_MAX; p++, i++)
		buf[i] = *p;
	buf[i] = '\0';
	
	return opendir(buf);
}

static Char *
g_strchr(str, ch)
	Char *str;
	int ch;
{
	do {
		if (*str == ch)
			return (str);
	} while (*str++);
	return (NULL);
}