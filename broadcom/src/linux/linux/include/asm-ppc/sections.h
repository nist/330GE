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
 * BK Id: SCCS/s.sections.h 1.11 09/08/01 15:47:43 paulus
 */
#ifdef __KERNEL__
#ifndef _PPC_SECTIONS_H
#define _PPC_SECTIONS_H

#define __pmac __attribute__ ((__section__ (".text.pmac")))
#define __pmacdata __attribute__ ((__section__ (".data.pmac")))
#define __pmacfunc(__argpmac) \
	__argpmac __pmac; \
	__argpmac
	
#define __prep __attribute__ ((__section__ (".text.prep")))
#define __prepdata __attribute__ ((__section__ (".data.prep")))
#define __prepfunc(__argprep) \
	__argprep __prep; \
	__argprep

#define __chrp __attribute__ ((__section__ (".text.chrp")))
#define __chrpdata __attribute__ ((__section__ (".data.chrp")))
#define __chrpfunc(__argchrp) \
	__argchrp __chrp; \
	__argchrp

/* this is actually just common chrp/pmac code, not OF code -- Cort */
#define __openfirmware __attribute__ ((__section__ (".text.openfirmware")))
#define __openfirmwaredata __attribute__ ((__section__ (".data.openfirmware")))
#define __openfirmwarefunc(__argopenfirmware) \
	__argopenfirmware __openfirmware; \
	__argopenfirmware
	
#endif /* _PPC_SECTIONS_H */
#endif /* __KERNEL__ */
