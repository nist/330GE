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
/* $Id: devops.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * devops.c:  Device operations using the PROM.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

extern void restore_current(void);

/* Open the device described by the string 'dstr'.  Returns the handle
 * to that device used for subsequent operations on that device.
 * Returns -1 on failure.
 */
int
prom_devopen(char *dstr)
{
	int handle;
	unsigned long flags;
	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
		handle = (*(romvec->pv_v0devops.v0_devopen))(dstr);
		if(handle == 0) handle = -1;
		break;
	case PROM_V2:
	case PROM_V3:
		handle = (*(romvec->pv_v2devops.v2_dev_open))(dstr);
		break;
	default:
		handle = -1;
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);

	return handle;
}

/* Close the device described by device handle 'dhandle'. */
int
prom_devclose(int dhandle)
{
	unsigned long flags;
	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
		(*(romvec->pv_v0devops.v0_devclose))(dhandle);
		break;
	case PROM_V2:
	case PROM_V3:
		(*(romvec->pv_v2devops.v2_dev_close))(dhandle);
		break;
	default:
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return 0;
}

/* Seek to specified location described by 'seekhi' and 'seeklo'
 * for device 'dhandle'.
 */
void
prom_seek(int dhandle, unsigned int seekhi, unsigned int seeklo)
{
	unsigned long flags;
	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
		(*(romvec->pv_v0devops.v0_seekdev))(dhandle, seekhi, seeklo);
		break;
	case PROM_V2:
	case PROM_V3:
		(*(romvec->pv_v2devops.v2_dev_seek))(dhandle, seekhi, seeklo);
		break;
	default:
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);

	return;
}
