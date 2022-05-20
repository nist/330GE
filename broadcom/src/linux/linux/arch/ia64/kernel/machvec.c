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
#include <linux/config.h>

#ifdef CONFIG_IA64_GENERIC

#include <linux/kernel.h>
#include <linux/string.h>

#include <asm/page.h>
#include <asm/machvec.h>

struct ia64_machine_vector ia64_mv;

/*
 * Most platforms use this routine for mapping page frame addresses
 * into a memory map index.
 */
unsigned long
map_nr_dense (unsigned long addr)
{
	return MAP_NR_DENSE(addr);
}

static struct ia64_machine_vector *
lookup_machvec (const char *name)
{
	extern struct ia64_machine_vector machvec_start[];
	extern struct ia64_machine_vector machvec_end[];
	struct ia64_machine_vector *mv;

	for (mv = machvec_start; mv < machvec_end; ++mv)
		if (strcmp (mv->name, name) == 0)
			return mv;

	return 0;
}

void
machvec_init (const char *name)
{
	struct ia64_machine_vector *mv;

	mv = lookup_machvec(name);
	if (!mv) {
		panic("generic kernel failed to find machine vector for platform %s!", name);
	}
	ia64_mv = *mv;
	printk("booting generic kernel on platform %s\n", name);
}

#endif /* CONFIG_IA64_GENERIC */

void
machvec_noop (void)
{
}
