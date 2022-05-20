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
 * linux/include/asm-arm/arch-l7200/aux_reg.h
 *
 * Copyright (C) 2000 Steve Hill (sjhill@cotw.com)
 *
 * Changelog:
 *   08-02-2000	SJH	Created file
 */
#ifndef _ASM_ARCH_AUXREG_H
#define _ASM_ARCH_AUXREG_H

#include <asm/arch/hardware.h>

#define l7200aux_reg	*((volatile unsigned int *) (AUX_BASE))

/*
 * Auxillary register values
 */
#define AUX_CLEAR		0x00000000
#define AUX_DIAG_LED_ON		0x00000002
#define AUX_RTS_UART1		0x00000004
#define AUX_DTR_UART1		0x00000008
#define AUX_KBD_COLUMN_12_HIGH	0x00000010
#define AUX_KBD_COLUMN_12_OFF	0x00000020
#define AUX_KBD_COLUMN_13_HIGH	0x00000040
#define AUX_KBD_COLUMN_13_OFF	0x00000080

#endif
