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
 *  linux/arch/arm/mm/mm-lusl7200.c
 *
 *  Copyright (C) 2000 Steve Hill (sjhill@cotw.com)
 *
 *  Extra MM routines for L7200 architecture
 */
#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/page.h>
#include <asm/proc/domain.h>

#include <asm/mach/map.h>

static struct map_desc l7200_io_desc[] __initdata = {
	{ IO_BASE,	IO_START,	IO_SIZE,	DOMAIN_IO, 0, 1 ,0 ,0},
	{ IO_BASE_2,	IO_START_2,	IO_SIZE_2,	DOMAIN_IO, 0, 1 ,0 ,0},
	{ AUX_BASE,     AUX_START,      AUX_SIZE,       DOMAIN_IO, 0, 1 ,0 ,0},
	{ FLASH1_BASE,  FLASH1_START,   FLASH1_SIZE,    DOMAIN_IO, 0, 1 ,0 ,0},
	{ FLASH2_BASE,  FLASH2_START,   FLASH2_SIZE,    DOMAIN_IO, 0, 1 ,0 ,0},
	LAST_DESC
};

void __init l7200_map_io(void)
{
	iotable_init(l7200_io_desc);
}
