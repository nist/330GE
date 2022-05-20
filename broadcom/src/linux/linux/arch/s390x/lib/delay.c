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
 *  arch/s390x/kernel/delay.c
 *    Precise Delay Loops for S390
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *
 *  Derived from "arch/i386/lib/delay.c"
 *    Copyright (C) 1993 Linus Torvalds
 *    Copyright (C) 1997 Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 */

#include <linux/config.h>
#include <linux/sched.h>
#include <linux/delay.h>

#ifdef CONFIG_SMP
#include <asm/smp.h>
#endif

void __delay(unsigned long loops)
{
        /*
         * To end the bloody studid and useless discussion about the
         * BogoMips number I took the liberty to define the __delay
         * function in a way that that resulting BogoMips number will
         * yield the megahertz number of the cpu. The important function
         * is udelay and that is done using the tod clock. -- martin.
         */
        __asm__ __volatile__(
                "0: brct %0,0b"
                : /* no outputs */ : "r" (loops/2) );
}

/*
 * Waits for 'usecs' microseconds using the tod clock
 */
void __udelay(unsigned long usecs)
{
        uint64_t start_cc, end_cc;

        if (usecs == 0)
                return;
        asm volatile ("STCK %0" : "=m" (start_cc));
        do {
                asm volatile ("STCK %0" : "=m" (end_cc));
        } while (((end_cc - start_cc)/4096) < usecs);
}

