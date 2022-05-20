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
 * linux/arch/arm/mach-sa1100/omnimeter.c
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


static void __init
fixup_omnimeter(struct machine_desc *desc, struct param_struct *params,
		char **cmdline, struct meminfo *mi)
{
	SET_BANK( 0, 0xc0000000, 16*1024*1024 );
	mi->nr_banks = 1;

	ROOT_DEV = MKDEV(RAMDISK_MAJOR,0);
	setup_ramdisk( 1, 0, 0, 8192 );
	setup_initrd( __phys_to_virt(0xd0000000), 0x00400000 );
}

static struct map_desc omnimeter_io_desc[] __initdata = {
 /* virtual     physical    length      domain     r  w  c  b */
  { 0xd2000000, 0x10000000, 0x02000000, DOMAIN_IO, 0, 1, 0, 0 }, /* TS */
  LAST_DESC
};

static void __init omnimeter_map_io(void)
{
	sa1100_map_io();
	iotable_init(omnimeter_io_desc);

	sa1100_register_uart(0, 3);
	sa1100_register_uart(1, 1);
}

MACHINE_START(OMNIMETER, "OmniMeter")
	BOOT_MEM(0xc0000000, 0x80000000, 0xf8000000)
	FIXUP(fixup_omnimeter)
	MAPIO(omnimeter_map_io)
	INITIRQ(sa1100_init_irq)
MACHINE_END
