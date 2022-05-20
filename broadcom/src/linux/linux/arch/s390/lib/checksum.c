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
 *  arch/s390/lib/checksum.c
 *    S390 fast network checksum routines
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Ulrich Hild        (first version),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com),
 *               Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 *
 * This file contains network checksum routines
 */
 
#include <linux/string.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <asm/byteorder.h>
#include <asm/checksum.h>

/*
 * computes a partial checksum, e.g. for TCP/UDP fragments
 */
unsigned int
csum_partial (const unsigned char *buff, int len, unsigned int sum)
{
	register_pair rp;
	  /*
	   * Experiments with ethernet and slip connections show that buff
	   * is aligned on either a 2-byte or 4-byte boundary.
	   */
	rp.subreg.even = (unsigned long) buff;
	rp.subreg.odd = (unsigned long) len;
        __asm__ __volatile__ (
                "0:  cksm %0,%1\n"    /* do checksum on longs */
                "    jo   0b\n"
                : "+&d" (sum), "+&a" (rp) : : "cc" );
        return sum;
}

/*
 *	Fold a partial checksum without adding pseudo headers
 */
unsigned short csum_fold(unsigned int sum)
{
	register_pair rp;

	__asm__ __volatile__ (
		"    slr  %N1,%N1\n" /* %0 = H L */
		"    lr   %1,%0\n"   /* %0 = H L, %1 = H L 0 0 */
		"    srdl %1,16\n"   /* %0 = H L, %1 = 0 H L 0 */
		"    alr  %1,%N1\n"  /* %0 = H L, %1 = L H L 0 */
		"    alr  %0,%1\n"   /* %0 = H+L+C L+H */
		"    srl  %0,16\n"   /* %0 = H+L+C */
		: "+&d" (sum), "=d" (rp) : : "cc" );
	return ((unsigned short) ~sum);
}

