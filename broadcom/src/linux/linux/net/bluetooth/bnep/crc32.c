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


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/atomic.h>

#include "crc32.h"

#define CRCPOLY_BE 0x04c11db7
#define CRC_BE_BITS 8

static u32 *bnep_crc32_table;

/*
 * This code is in the public domain; copyright abandoned.
 * Liability for non-performance of this code is limited to the amount
 * you paid for it.  Since it is distributed for free, your refund will
 * be very very small.  If it breaks, you get to keep both pieces.
 */
u32 bnep_crc32(u32 crc, unsigned char const *p, size_t len)
{
	while (len--)
		crc = (crc << 8) ^ bnep_crc32_table[(crc >> 24) ^ *p++];
	
	return crc;
}

int __init bnep_crc32_init(void)
{
	unsigned i, j;
	u32 crc = 0x80000000;

	bnep_crc32_table = kmalloc((1 << CRC_BE_BITS) * sizeof(u32), GFP_KERNEL);
	if (!bnep_crc32_table)
		return -ENOMEM;

	bnep_crc32_table[0] = 0;

	for (i = 1; i < 1 << CRC_BE_BITS; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		for (j = 0; j < i; j++)
			bnep_crc32_table[i + j] = crc ^ bnep_crc32_table[j];
	}
	return 0;
}

void __exit bnep_crc32_cleanup(void)
{
	if (bnep_crc32_table)
		kfree(bnep_crc32_table);
	bnep_crc32_table = NULL;
}
