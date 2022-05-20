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
#define LASATINT_END 16

/* lasat 100 */
#define LASAT_INT_STATUS_REG_100	(KSEG1ADDR(0x1c880000))
#define LASAT_INT_MASK_REG_100		(KSEG1ADDR(0x1c890000))
#define LASATINT_MASK_SHIFT_100		0

/* lasat 200 */
#define LASAT_INT_STATUS_REG_200	(KSEG1ADDR(0x1104003c))
#define LASAT_INT_MASK_REG_200		(KSEG1ADDR(0x1104003c))
#define LASATINT_MASK_SHIFT_200		16

