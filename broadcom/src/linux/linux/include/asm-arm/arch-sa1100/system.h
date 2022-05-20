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
 * linux/include/asm-arm/arch-sa1100/system.h
 *
 * Copyright (c) 1999 Nicolas Pitre <nico@cam.org>
 */
#include <linux/config.h>
#include <asm/arch/hardware.h>

static inline void arch_idle(void)
{
	if (!hlt_counter) {
		int flags;
		local_irq_save(flags);
		if (!current->need_resched)
			cpu_do_idle(0);
		local_irq_restore(flags);
	}
}

#ifdef CONFIG_SA1100_VICTOR

/* power off unconditionally */
#define arch_reset(x) machine_power_off()

#else

static inline void arch_reset(char mode)
{
	if (mode == 's') {
		/* Jump into ROM at address 0 */
		cpu_reset(0);
	} else {
		/* Use on-chip reset capability */
		RSRR = RSRR_SWR;
	}
}

#endif
