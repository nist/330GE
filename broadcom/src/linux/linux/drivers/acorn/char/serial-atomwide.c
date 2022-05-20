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
 *  linux/arch/arm/drivers/char/serial-atomwide.c
 *
 *  Copyright (C) 1996 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   02-05-1996	RMK	Created
 *   07-05-1996	RMK	Altered for greater number of cards.
 *   30-07-1996	RMK	Now uses generic card code.
 */
#define MY_CARD_LIST { MANU_ATOMWIDE, PROD_ATOMWIDE_3PSERIAL }
#define MY_NUMPORTS 3
#define MY_BAUD_BASE (7372800 / 16)
#define MY_BASE_ADDRESS(ec) \
	ecard_address ((ec), ECARD_IOC, ECARD_SLOW) + (0x2000 >> 2)
#define MY_PORT_ADDRESS(port,cardaddr) \
	((cardaddr) + 0x200 - (port) * 0x100)

#define INIT serial_card_atomwide_init
#define EXIT serial_card_atomwide_exit

#include "serial-card.c"
