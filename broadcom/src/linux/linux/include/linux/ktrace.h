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
 * Simple kernel trace
 * $Id: ktrace.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 */

#ifndef _LINUX_KTRACE_H
#define _LINUX_KTRACE_H

#include <linux/config.h>

#ifdef CONFIG_KTRACE

/* kernel subsystems */
#define	KT_SYSCALL	0x1
#define	KT_TRAP		0x2
#define	KT_PROC		0x4
#define	KT_IRQ		0x8
#define	KT_MM		0x10
#define	KT_SOCK		0x20
#define	KT_NET		0x40
#define	KT_FS		0x80

extern int ktracectl;
extern void _ktrace(char *fmt, unsigned long a1, unsigned long a2);

#define	ktrace(subsys, fmt, a1, a2)	if ((subsys) & ktracectl) _ktrace(fmt, (unsigned long)a1, (unsigned long)a2)

#else
#define	ktrace(subsys, fmt, a1, a2)
#endif	/* CONFIG_KTRACE */

#endif	/* _LINUX_KTRACE_H */
