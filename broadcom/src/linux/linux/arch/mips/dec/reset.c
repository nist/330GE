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
 * Reset a DECstation machine.
 *
 * Copyright (C) 199x  the Anonymous
 * Copyright (C) 2001, 2002, 2003  Maciej W. Rozycki
 */

#include <asm/addrspace.h>
#include <asm/ptrace.h>

#define back_to_prom()	(((void (*)(void))KSEG1ADDR(0x1fc00000))())

void dec_machine_restart(char *command)
{
	back_to_prom();
}

void dec_machine_halt(void)
{
	back_to_prom();
}

void dec_machine_power_off(void)
{
    /* DECstations don't have a software power switch */
	back_to_prom();
}

void dec_intr_halt(int irq, void *dev_id, struct pt_regs *regs)
{
	dec_machine_halt();
}
