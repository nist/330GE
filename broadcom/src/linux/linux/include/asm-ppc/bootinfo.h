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
 * BK Id: %F% %I% %G% %U% %#%
 */
/*
 * Non-machine dependent bootinfo structure.  Basic idea
 * borrowed from the m68k.
 *
 * Copyright (C) 1999 Cort Dougan <cort@ppc.kernel.org>
 */

#ifdef __KERNEL__
#ifndef _PPC_BOOTINFO_H
#define _PPC_BOOTINFO_H

#include <linux/config.h>

#if defined(CONFIG_APUS) && !defined(__BOOTER__)
#include <asm-m68k/bootinfo.h>
#else

struct bi_record {
    unsigned long tag;			/* tag ID */
    unsigned long size;			/* size of record (in bytes) */
    unsigned long data[0];		/* data */
};

#define BI_FIRST		0x1010  /* first record - marker */
#define BI_LAST			0x1011	/* last record - marker */
#define BI_CMD_LINE		0x1012
#define BI_BOOTLOADER_ID	0x1013
#define BI_INITRD		0x1014
#define BI_SYSMAP		0x1015
#define BI_MACHTYPE		0x1016
#define BI_MEMSIZE		0x1017

extern struct bi_record *find_bootinfo(void);
extern void parse_bootinfo(struct bi_record *rec);
extern unsigned long boot_mem_size;

#endif /* CONFIG_APUS */


#endif /* _PPC_BOOTINFO_H */
#endif /* __KERNEL__ */
