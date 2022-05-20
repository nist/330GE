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
 * File...........: linux/include/asm/todclk.h
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 1999,2000
 *
 * History of changes (starts July 2000)
 */

#ifndef __ASM_TODCLK_H
#define __ASM_TODCLK_H

#ifdef __KERNEL__

#define TOD_uSEC (0x1000ULL)
#define TOD_mSEC (1000 * TOD_uSEC)
#define TOD_SEC (1000 * TOD_mSEC)
#define TOD_MIN (60 * TOD_SEC)
#define TOD_HOUR (60 * TOD_MIN)

#endif

#endif
