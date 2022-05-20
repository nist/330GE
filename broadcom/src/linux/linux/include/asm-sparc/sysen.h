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
/* $Id: sysen.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * sysen.h:  Bit fields within the "System Enable" register accessed via
 *           the ASI_CONTROL address space at address AC_SYSENABLE.
 *
 * Copyright (C) 1994 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC_SYSEN_H
#define _SPARC_SYSEN_H

#define SENABLE_DVMA   0x20   /* enable dvma transfers */
#define SENABLE_CACHE  0x10   /* enable VAC cache */
#define SENABLE_RESET  0x04   /* reset whole machine, danger Will Robinson */

#endif /* _SPARC_SYSEN_H */
