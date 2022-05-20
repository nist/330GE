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
 *  linux/arch/x86_64/kernel/head64.c -- prepare to run common code
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 *
 *  $Id: head64.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 */

#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>

#include <asm/processor.h>
#include <asm/proto.h>

static void __init clear_bss(void)
{
	extern char __bss_start[], __bss_end[];
	printk("Clearing %ld bytes of bss...", (unsigned long) __bss_end - (unsigned long) __bss_start);
	memset(__bss_start, 0,
	       (unsigned long) __bss_end - (unsigned long) __bss_start);
	printk("ok\n");
}

extern char x86_boot_params[2048];

#define NEW_CL_POINTER		0x228	/* Relative to real mode data */
#define OLD_CL_MAGIC_ADDR	0x90020
#define OLD_CL_MAGIC            0xA33F
#define OLD_CL_BASE_ADDR        0x90000
#define OLD_CL_OFFSET           0x90022

extern char saved_command_line[];

static void __init copy_bootdata(char *real_mode_data)
{
	int new_data;
	char * command_line;

	memcpy(x86_boot_params, real_mode_data, 2048); 
	new_data = *(int *) (x86_boot_params + NEW_CL_POINTER);
	if (!new_data) {
		if (OLD_CL_MAGIC != * (u16 *) OLD_CL_MAGIC_ADDR) {
			printk("so old bootloader that it does not support commandline?!\n");
			return;
		}
		new_data = OLD_CL_BASE_ADDR + * (u16 *) OLD_CL_OFFSET;
		printk("old bootloader convention, maybe loadlin?\n");
	}
	command_line = (char *) ((u64)(new_data));
	memcpy(saved_command_line, command_line, 2048);
	printk("Bootdata ok (command line is %s)\n", saved_command_line);	
}

static void __init setup_boot_cpu_data(void)
{
	int dummy, eax;

	/* get vendor info */
	cpuid(0, &boot_cpu_data.cpuid_level,
	      (int *)&boot_cpu_data.x86_vendor_id[0],
	      (int *)&boot_cpu_data.x86_vendor_id[8],
	      (int *)&boot_cpu_data.x86_vendor_id[4]);

	/* get cpu type */
	cpuid(1, &eax, &dummy, &dummy, (int *) &boot_cpu_data.x86_capability);
	boot_cpu_data.x86 = (eax >> 8) & 0xf;
	boot_cpu_data.x86_model = (eax >> 4) & 0xf;
	boot_cpu_data.x86_mask = eax & 0xf;
}

void __init x86_64_start_kernel(char * real_mode_data)
{
	char *s; 

	clear_bss(); /* must be the first thing in C and must not depend on .bss to be zero */
	pda_init(0); 
	copy_bootdata(real_mode_data);
	s = strstr(saved_command_line, "earlyprintk="); 
	if (s != NULL)
		setup_early_printk(s+12); 
	early_printk("booting x86_64 kernel... ");
	setup_boot_cpu_data();
	start_kernel();
}
