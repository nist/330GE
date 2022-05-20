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
 * Copyright (C) 1996,1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

/* Open the device described by the string 'dstr'.  Returns the handle
 * to that device used for subsequent operations on that device.
 * Returns 0 on failure.
 */
int
prom_devopen(char *dstr)
{
	return p1275_cmd ("open", P1275_ARG(0,P1275_ARG_IN_STRING)|
				  P1275_INOUT(1,1),
				  dstr);
}

/* Close the device described by device handle 'dhandle'. */
int
prom_devclose(int dhandle)
{
	p1275_cmd ("close", P1275_INOUT(1,0), dhandle);
	return 0;
}

/* Seek to specified location described by 'seekhi' and 'seeklo'
 * for device 'dhandle'.
 */
void
prom_seek(int dhandle, unsigned int seekhi, unsigned int seeklo)
{
	p1275_cmd ("seek", P1275_INOUT(3,1), dhandle, seekhi, seeklo);
}
