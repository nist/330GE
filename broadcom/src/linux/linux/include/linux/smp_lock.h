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
#ifndef __LINUX_SMPLOCK_H
#define __LINUX_SMPLOCK_H

#include <linux/config.h>

#ifndef CONFIG_SMP

#define lock_kernel()				do { } while(0)
#define unlock_kernel()				do { } while(0)
#define release_kernel_lock(task, cpu)		do { } while(0)
#define reacquire_kernel_lock(task)		do { } while(0)
#define kernel_locked() 1

#else

#include <asm/smplock.h>

#endif /* CONFIG_SMP */

#endif
