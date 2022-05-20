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
 * linux/arch/arm/mach-sa1100/yopy.c
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/tty.h>

#include <asm/hardware.h>
#include <asm/setup.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/serial_sa1100.h>

#include "generic.h"


static spinlock_t egpio_lock = SPIN_LOCK_UNLOCKED;

static unsigned long yopy_egpio =
	GPIO_MASK(GPIO_CF_RESET) |
	GPIO_MASK(GPIO_CLKDIV_CLR1) | GPIO_MASK(GPIO_CLKDIV_CLR2) |
	GPIO_MASK(GPIO_SPEAKER_MUTE) | GPIO_MASK(GPIO_AUDIO_OPAMP_POWER);

int yopy_gpio_test(unsigned int gpio)
{
	return ((yopy_egpio & (1 << gpio)) != 0);
}

void yopy_gpio_set(unsigned int gpio, int level)
{
	unsigned long flags, mask;

	mask = 1 << gpio;

	spin_lock_irqsave(&egpio_lock, flags);

	if (level)
		yopy_egpio |= mask;
	else
		yopy_egpio &= ~mask;
	YOPY_EGPIO = yopy_egpio;

	spin_unlock_irqrestore(&egpio_lock, flags);
}

EXPORT_SYMBOL(yopy_gpio_test);
EXPORT_SYMBOL(yopy_gpio_set);

static int __init yopy_hw_init(void)
{
	if (machine_is_yopy()) {
		YOPY_EGPIO = yopy_egpio;

		/* Enable Output */
		PPDR |= PPC_L_BIAS;
		PSDR &= ~PPC_L_BIAS;
		PPSR |= PPC_L_BIAS;

		YOPY_EGPIO = yopy_egpio;
	}

	return 0;
}

__initcall(yopy_hw_init);


static struct map_desc yopy_io_desc[] __initdata = {
 /* virtual     physical    length      domain     r  w  c  b */
  { 0xe8000000, 0x00000000, 0x04000000, DOMAIN_IO, 0, 1, 0, 0 }, /* Flash 0 */
  { 0xec000000, 0x08000000, 0x04000000, DOMAIN_IO, 0, 1, 0, 0 }, /* Flash 1 */
  { 0xf0000000, 0x48000000, 0x00300000, DOMAIN_IO, 0, 1, 0, 0 }, /* LCD */
  { 0xf1000000, 0x10000000, 0x00100000, DOMAIN_IO, 0, 1, 0, 0 }, /* EGPIO */
  LAST_DESC
};

static void __init yopy_map_io(void)
{
	sa1100_map_io();
	iotable_init(yopy_io_desc);

	sa1100_register_uart(0, 3);

	set_GPIO_IRQ_edge(GPIO_UCB1200_IRQ, GPIO_RISING_EDGE);
}


MACHINE_START(YOPY, "Yopy")
	MAINTAINER("G.Mate, Inc.")
	BOOT_MEM(0xc0000000, 0x80000000, 0xf8000000)
	BOOT_PARAMS(0xc0000100)
	MAPIO(yopy_map_io)
	INITIRQ(sa1100_init_irq)
MACHINE_END
