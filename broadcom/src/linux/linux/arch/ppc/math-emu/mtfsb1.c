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
 * BK Id: SCCS/s.mtfsb1.c 1.6 05/17/01 18:14:23 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"

int
mtfsb1(int crbD)
{
	if ((crbD != 1) && (crbD != 2))
		__FPU_FPSCR |= (1 << (31 - crbD));

#ifdef DEBUG
	printk("%s: %d %08lx\n", __FUNCTION__, crbD, __FPU_FPSCR);
#endif

	return 0;
}
