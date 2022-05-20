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
#include <asm/bitops.h>

/* Find string of zero bits in a bitmap */ 
unsigned long 
find_next_zero_string(unsigned long *bitmap, long start, long nbits, int len)
{ 
	unsigned long n, end, i; 	

 again:
	n = find_next_zero_bit(bitmap, nbits, start);
	if (n == -1) 
		return -1;
	
	/* could test bitsliced, but it's hardly worth it */
	end = n+len;
	if (end >= nbits) 
		return -1; 
	for (i = n+1; i < end; i++) { 
		if (test_bit(i, bitmap)) {  
			start = i+1; 
			goto again; 
		} 
	}
	return n;
}
