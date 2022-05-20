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
 * BK Id: SCCS/s.amigahw.h 1.5 05/17/01 18:14:24 cort
 */
#ifdef __KERNEL__
#ifndef __ASMPPC_AMIGAHW_H
#define __ASMPPC_AMIGAHW_H

#include <linux/config.h>
#include <asm-m68k/amigahw.h>

#undef CHIP_PHYSADDR
#ifdef CONFIG_APUS_FAST_EXCEPT
#define CHIP_PHYSADDR      (0x000000)
#else
#define CHIP_PHYSADDR      (0x004000)
#endif


#endif /* __ASMPPC_AMIGAHW_H */
#endif /* __KERNEL__ */
