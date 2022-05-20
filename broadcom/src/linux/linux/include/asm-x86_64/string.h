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
#ifndef _X86_64_STRING_H_
#define _X86_64_STRING_H_

#ifdef __KERNEL__

#define struct_cpy(x,y) (*(x)=*(y))

/* Written 2002 by Andi Kleen */ 

/* Only used for special circumstances. Stolen from i386/string.h */ 
static inline void * __inline_memcpy(void * to, const void * from, size_t n)
{
unsigned long d0, d1, d2;
__asm__ __volatile__(
	"rep ; movsl\n\t"
	"testb $2,%b4\n\t"
	"je 1f\n\t"
	"movsw\n"
	"1:\ttestb $1,%b4\n\t"
	"je 2f\n\t"
	"movsb\n"
	"2:"
	: "=&c" (d0), "=&D" (d1), "=&S" (d2)
	:"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
	: "memory");
return (to);
}

/* Even with __builtin_ the compiler may decide to use the out of line
   function. */

#define __HAVE_ARCH_MEMCPY 1
extern void *__memcpy(void *to, const void *from, size_t len); 
#define memcpy(dst,src,len) \
	({ size_t __len = (len);				\
	   void *__ret;						\
	   if (__builtin_constant_p(len) && __len >= 64)	\
		 __ret = __memcpy((dst),(src),__len);		\
	   else							\
		 __ret = __builtin_memcpy((dst),(src),__len);	\
	   __ret; }) 


#define __HAVE_ARCH_MEMSET
#define memset __builtin_memset

#define __HAVE_ARCH_MEMMOVE
void * memmove(void * dest,const void *src,size_t count);

#endif /* __KERNEL__ */

#endif
