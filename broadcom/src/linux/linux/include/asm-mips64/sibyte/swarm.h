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
 * Copyright (C) 2000, 2001 ASUSTek Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef __ASM_SIBYTE_SWARM_H
#define __ASM_SIBYTE_SWARM_H

#include <asm/sibyte/sb1250.h>
#include <asm/sibyte/sb1250_int.h>

#ifdef CONFIG_SIBYTE_SWARM
#define SIBYTE_BOARD_NAME "BCM91250A (SWARM)"
#endif
#ifdef CONFIG_SIBYTE_PTSWARM
#define SIBYTE_BOARD_NAME "PTSWARM"
#endif
#ifdef CONFIG_SIBYTE_CRHONE
#define SIBYTE_BOARD_NAME "BCM91125C (CRhone)"
#endif
#ifdef CONFIG_SIBYTE_CRHINE
#define SIBYTE_BOARD_NAME "BCM91120C (CRhine)"
#endif

/* Generic bus chip selects */
#define LEDS_CS         3
#define LEDS_PHYS       0x100a0000
#if defined(CONFIG_SIBYTE_SWARM) || defined(CONFIG_SIBYTE_PTSWARM)
#define IDE_CS          4
#define IDE_PHYS        0x100b0000
#define PCMCIA_CS       6
#define PCMCIA_PHYS     0x11000000
#endif

/* GPIOs */
#if defined(CONFIG_SIBYTE_SWARM) || defined(CONFIG_SIBYTE_PTSWARM)
#define K_GPIO_GB_IDE   4
#define K_INT_GB_IDE    (K_INT_GPIO_0 + K_GPIO_GB_IDE)
#define K_GPIO_PC_READY 9
#define K_INT_PC_READY  (K_INT_GPIO_0 + K_GPIO_PC_READY)
#endif

#endif /* __ASM_SIBYTE_SWARM_H */
