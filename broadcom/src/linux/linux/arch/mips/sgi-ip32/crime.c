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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 Keith M Wesolowski
 */
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/ip32/crime.h>
#include <asm/ptrace.h>

void __init crime_init (void)
{
	u64 id = crime_read_64 (CRIME_ID);
	u64 rev = id & CRIME_ID_REV;

	id = (id & CRIME_ID_IDBITS) >> 4;

	printk ("CRIME id %1lx rev %ld detected at %016lx\n", id, rev,
		(unsigned long) CRIME_BASE);
}

void crime_memerr_intr (unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	u64 memerr = crime_read_64 (CRIME_MEM_ERROR_STAT);
	u64 addr = crime_read_64 (CRIME_MEM_ERROR_ADDR);
	memerr &= CRIME_MEM_ERROR_STAT_MASK;

	printk ("CRIME memory error at physaddr 0x%08lx status %08lx\n",
		addr << 2, memerr);

	crime_write_64 (CRIME_MEM_ERROR_STAT, 0);
}

void crime_cpuerr_intr (unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	u64 cpuerr = crime_read_64 (CRIME_CPU_ERROR_STAT);
	u64 addr = crime_read_64 (CRIME_CPU_ERROR_ADDR);
	cpuerr &= CRIME_CPU_ERROR_MASK;
	addr <<= 2UL;

	printk ("CRIME CPU interface error detected at %09lx status %08lx\n",
		addr, cpuerr);

	crime_write_64 (CRIME_CPU_ERROR_STAT, 0);
}
