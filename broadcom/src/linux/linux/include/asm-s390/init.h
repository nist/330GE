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
 *  include/asm-s390/init.h
 *
 *  S390 version
 */

#ifndef _S390_INIT_H
#define _S390_INIT_H

#define __init __attribute__ ((constructor))

/* don't know, if need on S390 */
#define __initdata
#define __initfunc(__arginit) \
        __arginit __init; \
        __arginit
/* For assembly routines
 * need to define ?
 */
/*
#define __INIT          .section        ".text.init",#alloc,#execinstr
#define __FINIT .previous
#define __INITDATA      .section        ".data.init",#alloc,#write
*/

#define __cacheline_aligned __attribute__ ((__aligned__(256)))

#endif

