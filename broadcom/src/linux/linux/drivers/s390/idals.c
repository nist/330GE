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
   * File...........: linux/drivers/s390x/idals.c
   * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
   * Bugreports.to..: <Linux390@de.ibm.com>
   * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 2000a
   
   * History of changes
   * 07/24/00 new file
   * 12/13/00 changed IDALs to 4kByte-IDALs
 */

#include <linux/module.h>
#include <linux/config.h>
#include <linux/slab.h>

#include <asm/irq.h>
#include <asm/idals.h>

#ifdef CONFIG_ARCH_S390X

unsigned long __create_idal (unsigned long address, int count)
{
	int nridaws;
	unsigned long *idal, *tmp;

        nridaws = ((address & (IDA_BLOCK_SIZE-1)) + count + 
		   (IDA_BLOCK_SIZE-1)) >> IDA_SIZE_LOG;
	idal = idal_alloc(nridaws);
	if (idal != NULL) {
		tmp = idal;
		*tmp++ = address;
		address &= -IDA_BLOCK_SIZE;
		while (--nridaws > 0) {
			address += IDA_BLOCK_SIZE;
			*tmp++ = address;
		}
	}
	return (unsigned long) idal;
}

EXPORT_SYMBOL (__create_idal);

#endif
