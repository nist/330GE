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
 * Copyright (C) 2001 by Ralf Baechle
 * Copyright (C) 2001 MIPS Technologies, Inc.
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/reboot.h>
#include <asm/reboot.h>

/*
 * Urgs ...  Too many MIPS machines to handle this in a generic way.
 * So handle all using function pointers to machine specific
 * functions.
 */
void (*_machine_restart)(char *command);
void (*_machine_halt)(void);
void (*_machine_power_off)(void);

void machine_restart(char *command)
{
	_machine_restart(command);
}

void machine_halt(void)
{
	_machine_halt();
}

void machine_power_off(void)
{
	_machine_power_off();
}
