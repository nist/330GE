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
 * arch/mips/sni/pcimt_scache.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1997, 1998 by Ralf Baechle
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/bcache.h>
#include <asm/sni.h>

#define cacheconf (*(volatile unsigned int *)PCIMT_CACHECONF)
#define invspace (*(volatile unsigned int *)PCIMT_INVSPACE)

void __init sni_pcimt_sc_init(void)
{
	unsigned int scsiz, sc_size;

	scsiz = cacheconf & 7;
	if (scsiz == 0) {
		printk("Second level cache is deactived.\n");
		return;
	}
	if (scsiz >= 6) {
		printk("Invalid second level cache size configured, "
		       "deactivating second level cache.\n");
		cacheconf = 0;
		return;
	}

	sc_size = 128 << scsiz;
	printk("%dkb second level cache detected, deactivating.\n", sc_size);
	cacheconf = 0;
}
