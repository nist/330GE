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
#ifndef _ASM_IA64_CHECKSUM_H
#define _ASM_IA64_CHECKSUM_H

/*
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 */

/*
 * This is a version of ip_compute_csum() optimized for IP headers,
 * which always checksum on 4 octet boundaries.
 */
extern unsigned short ip_fast_csum (unsigned char * iph, unsigned int ihl);

/*
 * Computes the checksum of the TCP/UDP pseudo-header returns a 16-bit
 * checksum, already complemented
 */
extern unsigned short int csum_tcpudp_magic (unsigned long saddr,
					     unsigned long daddr,
					     unsigned short len,
					     unsigned short proto,
					     unsigned int sum);

extern unsigned int csum_tcpudp_nofold (unsigned long saddr,
					unsigned long daddr,
					unsigned short len,
					unsigned short proto,
					unsigned int sum);

/*
 * Computes the checksum of a memory block at buff, length len,
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
extern unsigned int csum_partial (const unsigned char * buff, int len,
				  unsigned int sum);

/*
 * Same as csum_partial, but copies from src while it checksums.
 *
 * Here it is even more important to align src and dst on a 32-bit (or
 * even better 64-bit) boundary.
 */
extern unsigned int csum_partial_copy (const char *src, char *dst, int len,
				       unsigned int sum);

/*
 * The same as csum_partial, but copies from user space (but on the
 * ia-64 we have just one address space, so this is identical to the
 * above).
 *
 * This is obsolete and will go away.
 */
#define csum_partial_copy_fromuser csum_partial_copy

/*
 * This is a new version of the above that records errors it finds in
 * *errp, but continues and zeros the rest of the buffer.
 */
extern unsigned int csum_partial_copy_from_user (const char *src, char *dst,
						 int len, unsigned int sum,
						 int *errp);

extern unsigned int csum_partial_copy_nocheck (const char *src, char *dst,
					       int len, unsigned int sum);

/*
 * This routine is used for miscellaneous IP-like checksums, mainly in
 * icmp.c
 */
extern unsigned short ip_compute_csum (unsigned char *buff, int len);

/*
 * Fold a partial checksum without adding pseudo headers.
 */
static inline unsigned short
csum_fold (unsigned int sum)
{
	sum = (sum & 0xffff) + (sum >> 16);
	sum = (sum & 0xffff) + (sum >> 16);
	return ~sum;
}

#endif /* _ASM_IA64_CHECKSUM_H */
