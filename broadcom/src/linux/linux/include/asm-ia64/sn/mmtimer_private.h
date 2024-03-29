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
 * Intel Multimedia Timer device interface
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2001-2002 Silicon Graphics, Inc.  All rights reserved.
 *
 * Helper file for the SN implementation of mmtimers
 *
 * 11/01/01 - jbarnes - initial revision
 */

#ifndef _SN_MMTIMER_PRIVATE_H

#define RTC_BITS 55 /* 55 bits for this implementation */
#define NUM_COMPARATORS 2 /* two comparison registers in SN1 */

/*
 * Check for an interrupt and clear the pending bit if
 * one is waiting.
 */
#define MMTIMER_INT_PENDING(x) (x ? *(RTC_INT_PENDING_B_ADDR) : *(RTC_INT_PENDING_A_ADDR))

/*
 * Set interrupts on RTC 'x' to 'v' (true or false)
 */
#define MMTIMER_SET_INT(x,v) (x ? (*(RTC_INT_ENABLED_B_ADDR) = (unsigned long)(v)) : (*(RTC_INT_ENABLED_A_ADDR) = (unsigned long)(v)))

#define MMTIMER_ENABLE_INT(x) MMTIMER_SET_INT(x, 1)
#define MMTIMER_DISABLE_INT(x) MMTIMER_SET_INT(x, 0)

typedef struct mmtimer {
	spinlock_t timer_lock;
	unsigned long periodic;
	int signo;
	volatile unsigned long *compare;
	struct task_struct *process;
} mmtimer_t;

#endif /* _SN_LINUX_MMTIMER_PRIVATE_H */
