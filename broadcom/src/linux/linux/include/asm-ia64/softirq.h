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
#ifndef _ASM_IA64_SOFTIRQ_H
#define _ASM_IA64_SOFTIRQ_H

/*
 * Copyright (C) 1998-2001 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */
#include <asm/hardirq.h>

#define __local_bh_enable()	do { barrier(); really_local_bh_count()--; } while (0)

#define local_bh_disable()	do { really_local_bh_count()++; barrier(); } while (0)
#define local_bh_enable()								\
do {											\
	__local_bh_enable();								\
	if (__builtin_expect(local_softirq_pending(), 0) && really_local_bh_count() == 0)	\
		do_softirq();								\
} while (0)


#define in_softirq()		(really_local_bh_count() != 0)

#endif /* _ASM_IA64_SOFTIRQ_H */
