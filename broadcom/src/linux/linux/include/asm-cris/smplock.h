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
#ifndef __ASM_CRIS_SMPLOCK_H
#define __ASM_CRIS_SMPLOCK_H

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/config.h>

#ifndef CONFIG_SMP

#define lock_kernel()                           do { } while(0)
#define unlock_kernel()                         do { } while(0)
#define release_kernel_lock(task, cpu, depth)   ((depth) = 1)
#define reacquire_kernel_lock(task, cpu, depth) do { } while(0)

#else

#error "We do not support SMP on CRIS"

#endif

#endif
