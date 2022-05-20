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
/* Normally compiler builtins are used, but sometimes the compiler calls out
   of line code. Based on asm-i386/string.h.
 */
#define _STRING_C
#include <linux/string.h>

#undef memmove
void *memmove(void * dest,const void *src,size_t count)
{
	if (dest < src) { 
		__inline_memcpy(dest,src,count);
	} else {
		/* Could be more clever and move longs */
		unsigned long d0, d1, d2;
		__asm__ __volatile__(
			"std\n\t"
			"rep\n\t"
			"movsb\n\t"
			"cld"
			: "=&c" (d0), "=&S" (d1), "=&D" (d2)
			:"0" (count),
			"1" (count-1+(const char *)src),
			"2" (count-1+(char *)dest)
			:"memory");
	}
	return dest;
} 
