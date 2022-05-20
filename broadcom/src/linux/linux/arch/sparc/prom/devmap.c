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
/* $Id: devmap.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * promdevmap.c:  Map device/IO areas to virtual addresses.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

extern void restore_current(void);

/* Just like the routines in palloc.c, these should not be used
 * by the kernel at all.  Bootloader facility mainly.  And again,
 * this is only available on V2 proms and above.
 */

/* Map physical device address 'paddr' in IO space 'ios' of size
 * 'num_bytes' to a virtual address, with 'vhint' being a hint to
 * the prom as to where you would prefer the mapping.  We return
 * where the prom actually mapped it.
 */
char *
prom_mapio(char *vhint, int ios, unsigned int paddr, unsigned int num_bytes)
{
	unsigned long flags;
	char *ret;

	spin_lock_irqsave(&prom_lock, flags);
	if((num_bytes == 0) || (paddr == 0)) ret = (char *) 0x0;
	else
	ret = (*(romvec->pv_v2devops.v2_dumb_mmap))(vhint, ios, paddr,
						    num_bytes);
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return ret;
}

/* Unmap an IO/device area that was mapped using the above routine. */
void
prom_unmapio(char *vaddr, unsigned int num_bytes)
{
	unsigned long flags;

	if(num_bytes == 0x0) return;
	spin_lock_irqsave(&prom_lock, flags);
	(*(romvec->pv_v2devops.v2_dumb_munmap))(vaddr, num_bytes);
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return;
}
