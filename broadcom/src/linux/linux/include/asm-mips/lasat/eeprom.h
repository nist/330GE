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
#include <asm/addrspace.h>

/* lasat 100 */
#define AT93C_REG_100               KSEG1ADDR(0x1c810000)
#define AT93C_RDATA_REG_100         AT93C_REG_100
#define AT93C_RDATA_SHIFT_100       4
#define AT93C_WDATA_SHIFT_100       4
#define AT93C_CS_M_100              ( 1 << 5 )
#define AT93C_CLK_M_100             ( 1 << 3 )

/* lasat 200 */
#define AT93C_REG_200		KSEG1ADDR(0x11000000)
#define AT93C_RDATA_REG_200	(AT93C_REG_200+0x10000)
#define AT93C_RDATA_SHIFT_200	8
#define AT93C_WDATA_SHIFT_200	2
#define AT93C_CS_M_200		( 1 << 0 )
#define AT93C_CLK_M_200		( 1 << 1 )
