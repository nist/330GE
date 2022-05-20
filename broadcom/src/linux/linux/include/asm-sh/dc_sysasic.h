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
/* include/asm-sh/dc_sysasic.h
 *
 * Definitions for the Dreamcast System ASIC and related peripherals.
 *
 * Copyright (c) 2001 M. R. Brown <mrbrown@linuxdc.org>
 *
 * This file is part of the LinuxDC project (www.linuxdc.org)
 *
 * Released under the terms of the GNU GPL v2.0.
 *
 */

#include <asm/irq.h>

/* Hardware events -

   Each of these events correspond to a bit within the Event Mask Registers/
   Event Status Registers.  Because of the virtual IRQ numbering scheme, a
   base offset must be used when calculating the virtual IRQ that each event
   takes.
*/

#define HW_EVENT_IRQ_BASE  OFFCHIP_IRQ_BASE /* 48 */

/* IRQ 13 */
#define HW_EVENT_VSYNC     (HW_EVENT_IRQ_BASE +  5) /* VSync */
#define HW_EVENT_MAPLE_DMA (HW_EVENT_IRQ_BASE + 12) /* Maple DMA complete */
#define HW_EVENT_GDROM_DMA (HW_EVENT_IRQ_BASE + 14) /* GD-ROM DMA complete */

/* IRQ 11 */
#define HW_EVENT_GDROM_CMD (HW_EVENT_IRQ_BASE + 32) /* GD-ROM cmd. complete */
#define HW_EVENT_AICA_SYS  (HW_EVENT_IRQ_BASE + 33) /* AICA-related */
#define HW_EVENT_EXTERNAL  (HW_EVENT_IRQ_BASE + 35) /* Ext. (expansion) */

#define HW_EVENT_IRQ_MAX (HW_EVENT_IRQ_BASE + 95)
