/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * prototypes for functions defined in bcmstdlib.c
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bcmstdlib.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $:
 */

/*
 * bcmstdlib.h file should be used only to construct an OSL or alone without any OSL
 * It should not be used with any orbitarary OSL's as there could be a conflict
 * with some of the routines defined here.
*/

#ifndef	_BCMSTDLIB_H
#define	_BCMSTDLIB_H

#include <typedefs.h>
#include <bcmdefs.h>
#include <stdarg.h>

#ifndef INT_MAX
#define INT_MAX 2147483647 /* from limits.h */
#endif

#if !defined(NDIS) && !defined(_CFE_)

typedef int FILE;
#define stdout ((FILE *)1)
#define stderr ((FILE *)2)

/* i/o functions */
extern int fputc(int c, FILE *stream);
extern void putc(int c);
/* extern int putc(int c, FILE *stream); */
#define putchar(c) putc(c)
extern int fputs(const char *s, FILE *stream);
extern int puts(const char *s);
extern int getc(void);

/* string functions */
extern int printf(const char *fmt, ...);
extern int BCMROMFN(sprintf)(char *buf, const char *fmt, ...);

extern char *BCMROMFN(index)(const char *s, int c);

extern int BCMROMFN(strcmp)(const char *s1, const char *s2);
extern int BCMROMFN(strncmp)(const char *s1, const char *s2, size_t n);
extern char *BCMROMFN(strcpy)(char *dest, const char *src);
extern char *BCMROMFN(strncpy)(char *dest, const char *src, size_t n);
extern size_t BCMROMFN(strlen)(const char *s);
extern char *BCMROMFN(strchr)(const char *str, int c);
extern char *BCMROMFN(strrchr)(const char *str, int c);
extern char *BCMROMFN(strcat)(char *d, const char *s);
extern char *BCMROMFN(strstr)(const char *s, const char *find);
extern size_t BCMROMFN(strspn)(const char *s1, const char *s2);
extern size_t BCMROMFN(strcspn)(const char *s1, const char *s2);
extern unsigned long BCMROMFN(strtoul)(char *cp, char **endp, int base);
#define strtol(nptr, endptr, base) ((long)strtoul((nptr), (endptr), (base)))
#define	atoi(s)	((int)(strtoul((s), NULL, 10)))

/* mem functions */
/* Cannot use memmem in ROM because of character array initialization wiht "" in gcc */
extern void *memset(void *dest, int c, size_t n);
/* Cannot use memcpy in ROM because of structure assignmnets in gcc */
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *BCMROMFN(memmove)(void *dest, const void *src, size_t n);
extern void *BCMROMFN(memchr)(const void *s, int c, size_t n);
extern int BCMROMFN(memcmp)(const void *s1, const void *s2, size_t n);

extern int BCMROMFN(vsprintf)(char *buf, const char *fmt, va_list ap);

/* bcopy, bcmp, and bzero */
#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bcmp(b1, b2, len)	memcmp((b1), (b2), (len))
#define	bzero(b, len)		memset((b), '\0', (len))
extern unsigned long rand(void);

#endif 

extern int BCMROMFN(snprintf)(char *str, size_t n, char const *fmt, ...);
extern int BCMROMFN(vsnprintf)(char *buf, size_t size, const char *fmt, va_list ap);

#endif 	/* _BCMSTDLIB_H */