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
/* $Id: bootstr.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * bootstr.c:  Boot string/argument acquisition from the PROM.
 *
 * Copyright(C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright(C) 1996,1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/string.h>
#include <linux/init.h>
#include <asm/oplib.h>

/* WARNING: The boot loader knows that these next three variables come one right
 *          after another in the .data section.  Do not move this stuff into
 *          the .bss section or it will break things.
 */

#define BARG_LEN  256
int bootstr_len = BARG_LEN;
static int bootstr_valid = 0;
static char bootstr_buf[BARG_LEN] = { 0 };

char * __init
prom_getbootargs(void)
{
	/* This check saves us from a panic when bootfd patches args. */
	if (bootstr_valid) return bootstr_buf;
	prom_getstring(prom_chosen_node, "bootargs", bootstr_buf, BARG_LEN);
	bootstr_valid = 1;
	return bootstr_buf;
}
