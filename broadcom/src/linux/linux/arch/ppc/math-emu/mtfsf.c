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
 * BK Id: SCCS/s.mtfsf.c 1.6 05/17/01 18:14:23 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"

int
mtfsf(unsigned int FM, u32 *frB)
{
	u32 mask;

	if (FM == 0)
		return 0;

	if (FM == 0xff)
		mask = 0x9fffffff;
	else {
		mask = 0;
		if (FM & (1 << 0))
			mask |= 0x90000000;
		if (FM & (1 << 1))
			mask |= 0x0f000000;
		if (FM & (1 << 2))
			mask |= 0x00f00000;
		if (FM & (1 << 3))
			mask |= 0x000f0000;
		if (FM & (1 << 4))
			mask |= 0x0000f000;
		if (FM & (1 << 5))
			mask |= 0x00000f00;
		if (FM & (1 << 6))
			mask |= 0x000000f0;
		if (FM & (1 << 7))
			mask |= 0x0000000f;
	}

	__FPU_FPSCR &= ~(mask);
	__FPU_FPSCR |= (frB[1] & mask);

#ifdef DEBUG
	printk("%s: %02x %p: %08lx\n", __FUNCTION__, FM, frB, __FPU_FPSCR);
#endif

	return 0;
}
