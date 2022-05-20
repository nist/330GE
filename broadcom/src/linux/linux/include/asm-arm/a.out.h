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
#ifndef __ARM_A_OUT_H__
#define __ARM_A_OUT_H__

#include <linux/personality.h>
#include <asm/types.h>

struct exec
{
  __u32 a_info;		/* Use macros N_MAGIC, etc for access */
  __u32 a_text;		/* length of text, in bytes */
  __u32 a_data;		/* length of data, in bytes */
  __u32 a_bss;		/* length of uninitialized data area for file, in bytes */
  __u32 a_syms;		/* length of symbol table data in file, in bytes */
  __u32 a_entry;	/* start address */
  __u32 a_trsize;	/* length of relocation info for text, in bytes */
  __u32 a_drsize;	/* length of relocation info for data, in bytes */
};

/*
 * This is always the same
 */
#define N_TXTADDR(a)	(0x00008000)

#define N_TRSIZE(a)	((a).a_trsize)
#define N_DRSIZE(a)	((a).a_drsize)
#define N_SYMSIZE(a)	((a).a_syms)

#define M_ARM 103

#ifdef __KERNEL__
#define STACK_TOP	((current->personality == PER_LINUX_32BIT) ? \
			 TASK_SIZE : TASK_SIZE_26)
#endif

#ifndef LIBRARY_START_TEXT
#define LIBRARY_START_TEXT	(0x00c00000)
#endif

#endif /* __A_OUT_GNU_H__ */
