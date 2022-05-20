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
 * baget.h: Definitions specific to Baget/MIPS machines.
 *
 * Copyright (C) 1998 Gleb Raiko & Vladimir Roganov
 */
#ifndef _MIPS_BAGET_H
#define _MIPS_BAGET_H

#include "vic.h"
#include "vac.h"

#define VIC_BASE         0xBFFC0000
#define VAC_BASE         0xBFFD0000


/* Baget interrupt registers and their sizes */

struct  baget_int_reg {
	unsigned long address;
	int size;  /* in bytes */
};
#define BAGET_INT_NONE   {0,0}

#define BAGET_INT0_ACK   {0xbffa0003,1}
#define BAGET_INT1_ACK   {0xbffa0008,4}
#define BAGET_INT5_ACK   {0xbff00000,1}

#define BAGET_WRERR_ACK  ((volatile char*)0xbff00000)


/* Baget address spaces */

#define BAGET_A24M_BASE       0xFC000000      /* VME-master A24 base address  */
#define BAGET_A24S_BASE       0x00000000      /* VME-slave A24 base address   */
#define BAGET_A24S_MASK       0x00c00000      /* VME-slave A24 address mask   */
#define BAGET_GSW_BASE        0xf000          /* global switches address base */
#define BAGET_MSW_BASE(P) (0xe000+(P)*0x100)  /* module switches address base */

#define BAGET_LED_BASE  ((volatile short *)(0xbffd0000 + 0x00001800))

#define BAGET_PIL_NR            8
#define BAGET_IRQ_NR            NR_IRQS /* 64 */
#define BAGET_IRQ_MASK(x)       ((NR_IRQS-1) & (x))

#define BAGET_FPU_IRQ           0x26
#define BAGET_VIC_TIMER_IRQ     0x32
#define BAGET_VAC_TIMER_IRQ     0x36
#define BAGET_BSM_IRQ           0x3C

#define BAGET_LANCE_MEM_BASE    0xfcf10000
#define BAGET_LANCE_MEM_SIZE    0x10000
#define BAGET_LANCE_IO_BASE     0xbffeff00

#define BALO_OFFSET     0x400000 /* sync with ld.script.balo  */
#define BALO_SIZE       0x200000 /* sync with image segs size */

/* move it to the right place, somehere in include/asm */
#define CAUSE_DBE       0x1C
#define CAUSE_MASK      0x7C

/* Simple debug fascilities */
extern void outc(char);
extern void outs(char *);
extern void baget_write(char *s, int l);
extern int  baget_printk(const char *, ...);
extern void balo_printf( char *f, ... );
extern void balo_hungup(void);

#endif /* !(_MIPS_BAGET_H) */
