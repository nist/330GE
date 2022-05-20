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
 * linux/include/asm-mips/namei.h
 *
 * Included from linux/fs/namei.c
 */
#ifndef __ASM_NAMEI_H
#define __ASM_NAMEI_H

#include <linux/config.h>

/* Only one at this time. */
#define IRIX32_EMUL "usr/gnemul/irix/"

#ifdef CONFIG_BINFMT_IRIX

static inline char *__emul_prefix(void)
{
	if (current->personality != PER_IRIX32)
		return NULL;
	return IRIX32_EMUL;
}

#else /* !defined(CONFIG_BINFMT_IRIX) */

#define __emul_prefix() NULL

#endif /* !defined(CONFIG_BINFMT_IRIX) */

#endif /* __ASM_NAMEI_H */
