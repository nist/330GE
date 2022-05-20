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
/* $Id: string.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * string.h: External definitions for optimized assembly string
 *           routines for the Linux Kernel.
 *
 * Copyright (C) 1995,1996 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1996,1997,1999 Jakub Jelinek (jakub@redhat.com)
 */

#ifndef __SPARC64_STRING_H__
#define __SPARC64_STRING_H__

/* Really, userland/ksyms should not see any of this stuff. */

#ifdef __KERNEL__

#include <asm/asi.h>

extern void __memmove(void *,const void *,__kernel_size_t);
extern __kernel_size_t __memcpy(void *,const void *,__kernel_size_t);
extern void *__memset(void *,int,__kernel_size_t);
extern void *__builtin_memcpy(void *,const void *,__kernel_size_t);
extern void *__builtin_memset(void *,int,__kernel_size_t);

#ifndef EXPORT_SYMTAB_STROPS

/* First the mem*() things. */
#define __HAVE_ARCH_BCOPY
#define __HAVE_ARCH_MEMMOVE

#undef memmove
#define memmove(_to, _from, _n) \
({ \
	void *_t = (_to); \
	__memmove(_t, (_from), (_n)); \
	_t; \
})

#define __HAVE_ARCH_MEMCPY

static inline void *__constant_memcpy(void *to, const void *from, __kernel_size_t n)
{
	if(n) {
		if(n <= 32) {
			__builtin_memcpy(to, from, n);
		} else {
			__memcpy(to, from, n);
		}
	}
	return to;
}

static inline void *__nonconstant_memcpy(void *to, const void *from, __kernel_size_t n)
{
	__memcpy(to, from, n);
	return to;
}

#undef memcpy
#define memcpy(t, f, n) \
(__builtin_constant_p(n) ? \
 __constant_memcpy((t),(f),(n)) : \
 __nonconstant_memcpy((t),(f),(n)))

#define __HAVE_ARCH_MEMSET

static inline void *__constant_memset(void *s, int c, __kernel_size_t count)
{
	extern __kernel_size_t __bzero(void *, __kernel_size_t);

	if(!c) {
		__bzero(s, count);
		return s;
	} else
		return __memset(s, c, count);
}

#undef memset
#define memset(s, c, count) \
((__builtin_constant_p(count) && (count) <= 32) ? \
 __builtin_memset((s), (c), (count)) : \
 (__builtin_constant_p(c) ? \
  __constant_memset((s), (c), (count)) : \
  __memset((s), (c), (count))))

#define __HAVE_ARCH_MEMSCAN

#undef memscan
#define memscan(__arg0, __char, __arg2)						\
({										\
	extern void *__memscan_zero(void *, size_t);				\
	extern void *__memscan_generic(void *, int, size_t);			\
	void *__retval, *__addr = (__arg0);					\
	size_t __size = (__arg2);						\
										\
	if(__builtin_constant_p(__char) && !(__char))				\
		__retval = __memscan_zero(__addr, __size);			\
	else									\
		__retval = __memscan_generic(__addr, (__char), __size);		\
										\
	__retval;								\
})

#define __HAVE_ARCH_MEMCMP
extern int memcmp(const void *,const void *,__kernel_size_t);

/* Now the str*() stuff... */
#define __HAVE_ARCH_STRLEN

extern __kernel_size_t __strlen(const char *);
extern __kernel_size_t strlen(const char *);

#define __HAVE_ARCH_STRNCMP

extern int __strncmp(const char *, const char *, __kernel_size_t);

static inline int __constant_strncmp(const char *src, const char *dest, __kernel_size_t count)
{
	register int retval;
	switch(count) {
	case 0: return 0;
	case 1: return (src[0] - dest[0]);
	case 2: retval = (src[0] - dest[0]);
		if(!retval && src[0])
		  retval = (src[1] - dest[1]);
		return retval;
	case 3: retval = (src[0] - dest[0]);
		if(!retval && src[0]) {
		  retval = (src[1] - dest[1]);
		  if(!retval && src[1])
		    retval = (src[2] - dest[2]);
		}
		return retval;
	case 4: retval = (src[0] - dest[0]);
		if(!retval && src[0]) {
		  retval = (src[1] - dest[1]);
		  if(!retval && src[1]) {
		    retval = (src[2] - dest[2]);
		    if (!retval && src[2])
		      retval = (src[3] - dest[3]);
		  }
		}
		return retval;
	case 5: retval = (src[0] - dest[0]);
		if(!retval && src[0]) {
		  retval = (src[1] - dest[1]);
		  if(!retval && src[1]) {
		    retval = (src[2] - dest[2]);
		    if (!retval && src[2]) {
		      retval = (src[3] - dest[3]);
		      if (!retval && src[3])
		        retval = (src[4] - dest[4]);
		    }
		  }
		}
		return retval;
	default:
		retval = (src[0] - dest[0]);
		if(!retval && src[0]) {
		  retval = (src[1] - dest[1]);
		  if(!retval && src[1]) {
		    retval = (src[2] - dest[2]);
		    if(!retval && src[2])
		      retval = __strncmp(src+3,dest+3,count-3);
		  }
		}
		return retval;
	}
}

#undef strncmp
#define strncmp(__arg0, __arg1, __arg2)	\
(__builtin_constant_p(__arg2) ?	\
 __constant_strncmp(__arg0, __arg1, __arg2) : \
 __strncmp(__arg0, __arg1, __arg2))

#endif /* !EXPORT_SYMTAB_STROPS */

#endif /* __KERNEL__ */

#endif /* !(__SPARC64_STRING_H__) */