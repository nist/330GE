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
#ifndef _CRIS_DELAY_H
#define _CRIS_DELAY_H

/*
 * Copyright (C) 1998, 1999, 2000, 2001 Axis Communications AB
 *
 * Delay routines, using a pre-computed "loops_per_second" value.
 */

#include <linux/config.h>
#include <linux/linkage.h>

#ifdef CONFIG_SMP
#include <asm/smp.h>
#endif 

extern void __do_delay(void);	/* Special register call calling convention */

extern __inline__ void __delay(int loops)
{
	__asm__ __volatile__ (
			      "move.d %0,$r9\n\t"
			      "beq 2f\n\t"
			      "subq 1,$r9\n\t"
			      "1:\n\t"
			      "bne 1b\n\t"
			      "subq 1,$r9\n"
			      "2:"
			      : : "g" (loops) : "r9");
}


/* Use only for very small delays ( < 1 msec).  */

extern unsigned long loops_per_usec; /* arch/cris/mm/init.c */

extern __inline__ void udelay(unsigned long usecs)
{
	__delay(usecs * loops_per_usec);
}

#endif /* defined(_CRIS_DELAY_H) */



