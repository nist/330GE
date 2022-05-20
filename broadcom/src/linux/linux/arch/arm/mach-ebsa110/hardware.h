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
 *  linux/arch/arm/mach-ebsa110/hardware.h
 *
 *  Copyright (C) 2001 Russell King
 *
 *  Local hardware definitions.
 */
#ifndef HARDWARE_H
#define HARDWARE_H

#define IRQ_MASK		0xfe000000	/* read */
#define IRQ_MSET		0xfe000000	/* write */
#define IRQ_STAT		0xff000000	/* read */
#define IRQ_MCLR		0xff000000	/* write */

#endif
