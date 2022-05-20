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
#ifndef _PPC64_DELAY_H
#define _PPC64_DELAY_H

/*
 * Copyright 1996, Paul Mackerras.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * PPC64 Support added by Dave Engebretsen, Todd Inglett, Mike Corrigan,
 * Anton Blanchard.
 */

#ifndef __ASSEMBLY__
extern unsigned long tb_ticks_per_usec;

/* define these here to prevent circular dependencies */ 
#define __HMT_low()	asm volatile("or 1,1,1")
#define __HMT_medium()	asm volatile("or 2,2,2")

static inline unsigned long __get_tb(void)
{
	unsigned long rval;

	asm volatile("mftb %0" : "=r" (rval));
	return rval;
}

static inline void __delay(unsigned long loops)
{
	unsigned long start = __get_tb();

	while((__get_tb()-start) < loops)
		__HMT_low();
}

static inline void udelay(unsigned long usecs)
{
	unsigned long loops = tb_ticks_per_usec * usecs;

	__delay(loops);
	__HMT_medium();
}
#endif /* !__ASSEMBLY__ */

#endif /* _PPC64_DELAY_H */
