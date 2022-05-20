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
 * PROM library initialisation code.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 */
#include <linux/init.h>
#include <linux/kernel.h>

#include <asm/sgialib.h>

#undef DEBUG_PROM_INIT

/* Master romvec interface. */
struct linux_romvec *romvec;
int prom_argc;
LONG *_prom_argv, *_prom_envp;

void __init prom_init(int argc, char **argv, char **envp, int *prom_vec)
{
	PSYSTEM_PARAMETER_BLOCK pb = PROMBLOCK;
	romvec = ROMVECTOR;
	prom_argc = argc;
	_prom_argv = (LONG *) argv;
	_prom_envp = (LONG *) envp;

	if (pb->magic != 0x53435241) {
		prom_printf("Aieee, bad prom vector magic %08lx\n", pb->magic);
		while(1)
			;
	}

	prom_init_cmdline();
	prom_identify_arch();
	printk(KERN_INFO "PROMLIB: ARC firmware Version %d Revision %d\n",
	       pb->ver, pb->rev);
	prom_meminit();

#ifdef DEBUG_PROM_INIT
	prom_printf("Press a key to reboot\n");
	prom_getchar();
	ArcEnterInteractiveMode();
#endif
}
