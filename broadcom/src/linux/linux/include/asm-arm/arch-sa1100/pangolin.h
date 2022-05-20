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
 * linux/include/asm-arm/arch-sa1100/pangolin.h
 *
 * Created 2000/08/25 by Murphy Chen <murphy@mail.dialogue.com.tw>
 *
 * This file contains the hardware specific definitions for Pangolin
 *
 */

#ifndef __ASM_ARCH_HARDWARE_H
#error "include <asm/hardware.h> instead"
#endif

#include <linux/config.h>

#ifndef CONFIG_SA1100_PANGOLIN_PCMCIA_IDE

/* GPIOs for which the generic definition doesn't say much */
#define GPIO_CF_BUS_ON		GPIO_GPIO (3)
#define GPIO_CF_RESET		GPIO_GPIO (2)
#define GPIO_CF_CD		GPIO_GPIO (22)
#define GPIO_CF_IRQ		GPIO_GPIO (21)

#define IRQ_GPIO_CF_IRQ		IRQ_GPIO21
#define IRQ_GPIO_CF_CD		IRQ_GPIO22

#else
/*
 *  These definitions are for PCMCIA/IDE card
 *
 *  PSKTSEL = 0 ---> PCMCIA
 *  PCMCIA_RESET = GPIO_7        ( output )( 0: normal   1: reset )
 *  PCMCIA_IRQ = GPIO_24         ( input )
 *  PCMCIA_CD = GPIO_25          ( input )
 *
 *  PSKTSEL = 1 ---> IDE port
 *  IDE_IRQ = GPIO_23            ( input )
 *
 *  !!WARNING!!
 *  When the PCMCIA/IDE card is inserted, the CF slot
 *  should not have any card inserted!!
 *
 */

#define GPIO_PCMCIA_RESET       GPIO_GPIO (7)
#define GPIO_PCMCIA_IRQ         GPIO_GPIO (24)
#define GPIO_PCMCIA_CD          GPIO_GPIO (25)
#define GPIO_IDE_IRQ            GPIO_GPIO (8)

#define IRQ_PCMCIA_IRQ          IRQ_GPIO24
#define IRQ_PCMCIA_CD           IRQ_GPIO25
#define IRQ_IDE_IRQ             IRQ_GPIO8

#endif

/*
 * On board LAN chip
 */
#define PANGOLIN_LAN_ADDR      0x32000000
#define PANGOLIN_LAN_RESET     GPIO_GPIO (8)
#define PANGOLIN_LAN_IRQ       GPIO_GPIO (26)
#define PANGOLIN_IRQ_LAN_IRQ   IRQ_GPIO26

