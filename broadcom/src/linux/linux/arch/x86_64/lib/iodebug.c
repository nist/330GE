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
#include <asm/io.h>

void * __io_virt_debug(unsigned long x, const char *file, int line)
{
	if (x < PAGE_OFFSET) {
		printk("io mapaddr 0x%05lx not valid at %s:%d!\n", x, file, line);
		return __va(x);
	}
	return (void *)x;
}

unsigned long __io_phys_debug(unsigned long x, const char *file, int line)
{
	if (x < PAGE_OFFSET) {
		printk("io mapaddr 0x%05lx not valid at %s:%d!\n", x, file, line);
		return x;
	}
	return __pa(x);
}
