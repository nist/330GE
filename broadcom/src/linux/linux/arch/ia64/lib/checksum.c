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
 * Network checksum routines
 *
 * Copyright (C) 1999 Hewlett-Packard Co
 * Copyright (C) 1999 Stephane Eranian <eranian@hpl.hp.com>
 *
 * Most of the code coming from arch/alpha/lib/checksum.c
 *
 * This file contains network checksum routines that are better done
 * in an architecture-specific manner due to speed..
 */

#include <linux/string.h>

#include <asm/byteorder.h>

static inline unsigned short
from64to16(unsigned long x)
{
	/* add up 32-bit words for 33 bits */
	x = (x & 0xffffffff) + (x >> 32);
	/* add up 16-bit and 17-bit words for 17+c bits */
	x = (x & 0xffff) + (x >> 16);
	/* add up 16-bit and 2-bit for 16+c bit */
	x = (x & 0xffff) + (x >> 16);
	/* add up carry.. */
	x = (x & 0xffff) + (x >> 16);
	return x;
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented.
 */
unsigned short int csum_tcpudp_magic(unsigned long saddr,
				   unsigned long daddr,
				   unsigned short len,
				   unsigned short proto,
				   unsigned int sum)
{
	return ~from64to16(saddr + daddr + sum +
		((unsigned long) ntohs(len) << 16) +
		((unsigned long) proto << 8));
}

unsigned int csum_tcpudp_nofold(unsigned long saddr,
				   unsigned long daddr,
				   unsigned short len,
				   unsigned short proto,
				   unsigned int sum)
{
	unsigned long result;

	result = (saddr + daddr + sum +
		  ((unsigned long) ntohs(len) << 16) +
		  ((unsigned long) proto << 8));

	/* Fold down to 32-bits so we don't loose in the typedef-less network stack.  */
	/* 64 to 33 */
	result = (result & 0xffffffff) + (result >> 32);
	/* 33 to 32 */
	result = (result & 0xffffffff) + (result >> 32);
	return result;
}

extern unsigned long do_csum (const unsigned char *, long);

/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
 * it's best to have buff aligned on a 32-bit boundary
 */
unsigned int csum_partial(const unsigned char * buff, int len, unsigned int sum)
{
	unsigned long result = do_csum(buff, len);

	/* add in old sum, and carry.. */
	result += sum;
	/* 32+c bits -> 32 bits */
	result = (result & 0xffffffff) + (result >> 32);
	return result;
}


/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */
unsigned short ip_compute_csum(unsigned char * buff, int len)
{
	return ~do_csum(buff,len);
}
