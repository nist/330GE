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
/* $Id: mp.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * mp.c:  OpenBoot Prom Multiprocessor support routines.  Don't call
 *        these on a UP or else you will halt and catch fire. ;)
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

extern void restore_current(void);

int
prom_startcpu(int cpunode, struct linux_prom_registers *ctable_reg, int ctx, char *pc)
{
	int ret;
	unsigned long flags;

	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
	case PROM_V2:
	default:
		ret = -1;
		break;
	case PROM_V3:
		ret = (*(romvec->v3_cpustart))(cpunode, (int) ctable_reg, ctx, pc);
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);

	return ret;
}

int
prom_stopcpu(int cpunode)
{
	int ret;
	unsigned long flags;

	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
	case PROM_V2:
	default:
		ret = -1;
		break;
	case PROM_V3:
		ret = (*(romvec->v3_cpustop))(cpunode);
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);

	return ret;
}

int
prom_idlecpu(int cpunode)
{
	int ret;
	unsigned long flags;

	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
	case PROM_V2:
	default:
		ret = -1;
		break;
	case PROM_V3:
		ret = (*(romvec->v3_cpuidle))(cpunode);
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);

	return ret;
}

int
prom_restartcpu(int cpunode)
{
	int ret;
	unsigned long flags;

	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
	case PROM_V2:
	default:
		ret = -1;
		break;
	case PROM_V3:
		ret = (*(romvec->v3_cpuresume))(cpunode);
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);

	return ret;
}
