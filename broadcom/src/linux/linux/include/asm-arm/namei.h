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
 * linux/include/asm-arm/namei.h
 *
 * Routines to handle famous /usr/gnemul
 * Derived from the Sparc version of this file
 *
 * Included from linux/fs/namei.c
 */

#ifndef __ASMARM_NAMEI_H
#define __ASMARM_NAMEI_H

#define ARM_BSD_EMUL "usr/gnemul/bsd/"

static inline char *__emul_prefix(void)
{
	switch (current->personality) {
	case PER_BSD:
		return ARM_BSD_EMUL;
	default:
		return NULL;
	}
}

#endif /* __ASMARM_NAMEI_H */
