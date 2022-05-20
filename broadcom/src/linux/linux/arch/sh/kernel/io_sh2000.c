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
 * I/O routine for SH-2000
 */
#include <linux/config.h>
#include <asm/io.h>
#include <asm/machvec.h>

#define IDE_OFFSET    0xb6200000
#define NIC_OFFSET    0xb6000000
#define EXTBUS_OFFSET 0xba000000

unsigned long sh2000_isa_port2addr(unsigned long offset)
{
	if((offset & ~7) == 0x1f0 || offset == 0x3f6)
		return IDE_OFFSET + offset;
	else if((offset & ~0x1f) == 0x300)
		return NIC_OFFSET + offset;
	return EXTBUS_OFFSET + offset;
}
