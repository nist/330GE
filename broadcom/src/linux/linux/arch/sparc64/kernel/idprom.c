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
/* $Id: idprom.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * idprom.c: Routines to load the idprom into kernel addresses and
 *           interpret the data contained within.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>

#include <asm/oplib.h>
#include <asm/idprom.h>

struct idprom *idprom;
static struct idprom idprom_buffer;

/* Calculate the IDPROM checksum (xor of the data bytes). */
static unsigned char __init calc_idprom_cksum(struct idprom *idprom)
{
	unsigned char cksum, i, *ptr = (unsigned char *)idprom;

	for (i = cksum = 0; i <= 0x0E; i++)
		cksum ^= *ptr++;

	return cksum;
}

/* Create a local IDPROM copy and verify integrity. */
void __init idprom_init(void)
{
	prom_get_idprom((char *) &idprom_buffer, sizeof(idprom_buffer));

	idprom = &idprom_buffer;

	if (idprom->id_format != 0x01)  {
		prom_printf("IDPROM: Warning, unknown format type!\n");
	}

	if (idprom->id_cksum != calc_idprom_cksum(idprom)) {
		prom_printf("IDPROM: Warning, checksum failure (nvram=%x, calc=%x)!\n",
			    idprom->id_cksum, calc_idprom_cksum(idprom));
	}

	printk("Ethernet address: %02x:%02x:%02x:%02x:%02x:%02x\n",
	       idprom->id_ethaddr[0], idprom->id_ethaddr[1],
	       idprom->id_ethaddr[2], idprom->id_ethaddr[3],
	       idprom->id_ethaddr[4], idprom->id_ethaddr[5]);
}
