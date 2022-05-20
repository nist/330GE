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
 * linux/arch/arm/mach-sa1100/lart.c
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/tty.h>

#include <asm/hardware.h>
#include <asm/setup.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/serial_sa1100.h>

#include "generic.h"


static struct map_desc lart_io_desc[] __initdata = {
 /* virtual     physical    length      domain     r  w  c  b */
  { 0xe8000000, 0x00000000, 0x00400000, DOMAIN_IO, 0, 1, 0, 0 }, /* main flash memory */
  { 0xec000000, 0x08000000, 0x00400000, DOMAIN_IO, 0, 1, 0, 0 }, /* main flash, alternative location */
  LAST_DESC
};

static void __init lart_map_io(void)
{
	sa1100_map_io();
	iotable_init(lart_io_desc);

	sa1100_register_uart(0, 3);
	sa1100_register_uart(1, 1);
	sa1100_register_uart(2, 2);

	GAFR |= (GPIO_UART_TXD | GPIO_UART_RXD);
	GPDR |= GPIO_UART_TXD;
	GPDR &= ~GPIO_UART_RXD;
	PPAR |= PPAR_UPR;
}

MACHINE_START(LART, "LART")
	BOOT_MEM(0xc0000000, 0x80000000, 0xf8000000)
	BOOT_PARAMS(0xc0000100)
	MAPIO(lart_map_io)
	INITIRQ(sa1100_init_irq)
MACHINE_END
