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
 * Definitions for the MIPS sysmips(2) call
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995 by Ralf Baechle
 */
#ifndef __ASM_MIPS_SYSMIPS_H
#define __ASM_MIPS_SYSMIPS_H

/*
 * Commands for the sysmips(2) call
 *
 * sysmips(2) is deprecated - though some existing software uses it.
 * We only support the following commands.
 */
#define SETNAME                    1	/* set hostname                  */
#define FLUSH_CACHE		   3	/* writeback and invalidate caches */
#define MIPS_FIXADE                7	/* control address error fixing  */
#define MIPS_RDNVRAM              10	/* read NVRAM */
#define MIPS_ATOMIC_SET		2001	/* atomically set variable       */

#endif /* __ASM_MIPS_SYSMIPS_H */
