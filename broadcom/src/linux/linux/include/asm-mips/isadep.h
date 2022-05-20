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
 * Various ISA level dependant constants.
 * Most of the following constants reflect the different layout
 * of Coprocessor 0 registers.
 *
 * Copyright (c) 1998 Harald Koerfgen
 */
#include <linux/config.h>

#ifndef __ASM_ISADEP_H
#define __ASM_ISADEP_H

#if defined(CONFIG_CPU_R3000) || defined(CONFIG_CPU_TX39XX)
/*
 * R2000 or R3000
 */

/*
 * kernel or user mode? (CP0_STATUS)
 */
#define KU_MASK 0x08
#define	KU_USER 0x08
#define KU_KERN 0x00

#else
/*
 * kernel or user mode?
 */
#define KU_MASK 0x18
#define	KU_USER 0x10
#define KU_KERN 0x00

#endif

#endif /* __ASM_ISADEP_H */
