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
 */

#include <linux/string.h>
#include <asm/oplib.h>
#include <asm/sun4prom.h>
#include <linux/init.h>

#define BARG_LEN  256
static char barg_buf[BARG_LEN] = { 0 };
static char fetched __initdata = 0;

extern linux_sun4_romvec *sun4_romvec;

char * __init
prom_getbootargs(void)
{
	int iter;
	char *cp, *arg;

	/* This check saves us from a panic when bootfd patches args. */
	if (fetched) {
		return barg_buf;
	}

	switch(prom_vers) {
	case PROM_V0:
	case PROM_SUN4:
		cp = barg_buf;
		/* Start from 1 and go over fd(0,0,0)kernel */
		for(iter = 1; iter < 8; iter++) {
			arg = (*(romvec->pv_v0bootargs))->argv[iter];
			if(arg == 0) break;
			while(*arg != 0) {
				/* Leave place for space and null. */
				if(cp >= barg_buf + BARG_LEN-2){
					/* We might issue a warning here. */
					break;
				}
				*cp++ = *arg++;
			}
			*cp++ = ' ';
		}
		*cp = 0;
		break;
	case PROM_V2:
	case PROM_V3:
		/*
		 * V3 PROM cannot supply as with more than 128 bytes
		 * of an argument. But a smart bootstrap loader can.
		 */
		strncpy(barg_buf, *romvec->pv_v2bootargs.bootargs, BARG_LEN-1);
		break;
	default:
		break;
	}

	fetched = 1;
	return barg_buf;
}
