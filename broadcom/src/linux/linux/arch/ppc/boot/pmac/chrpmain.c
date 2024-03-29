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
 * BK Id: SCCS/s.chrpmain.c 1.18 01/11/02 10:46:07 trini
 */
/*
 * Copyright (C) Paul Mackerras 1997.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include "nonstdio.h"
#include <asm/processor.h>
#include <asm/page.h>

/* Passed from the linker */
extern char __image_begin, __image_end;
extern char __ramdisk_begin[], __ramdisk_end;
extern char _start, _end;

extern int getprop(void *, const char *, void *, int);
extern unsigned int heap_max;
extern void *claim(unsigned int virt, unsigned int size, unsigned int align);
extern void *finddevice(const char *);
extern void flush_cache(void *start, unsigned int len);
extern void gunzip(void *, int, unsigned char *, int *);
extern void make_bi_recs(unsigned long addr, char *name, unsigned int mach,
		unsigned int progend);
extern void pause(void);
extern void release(void *ptr, unsigned int len);

char *avail_ram;
char *begin_avail, *end_avail;
char *avail_high;


#define RAM_END		(16 << 20)

#define PROG_START	0x00010000
#define PROG_SIZE	0x003f0000

#define SCRATCH_SIZE	(128 << 10)

void boot(int a1, int a2, void *prom)
{
    unsigned sa, len;
    void *dst;
    unsigned char *im;
    unsigned initrd_start, initrd_size;
    
    printf("chrpboot starting: loaded at 0x%p\n", &_start);

    initrd_size = (char *)(&__ramdisk_end) - (char *)(&__ramdisk_begin);
    if (initrd_size) {
	initrd_start = (RAM_END - initrd_size) & ~0xFFF;
	a1 = initrd_start;
	a2 = initrd_size;
	claim(initrd_start, RAM_END - initrd_start, 0);
	printf("initial ramdisk moving 0x%x <- 0x%p (%x bytes)\n\r",
	       initrd_start, (char *)(&__ramdisk_begin), initrd_size);
	memcpy((char *)initrd_start, (char *)(&__ramdisk_begin), initrd_size);
    } else
	a2 = 0xdeadbeef;

    im = (char *)(&__image_begin);
    len = (char *)(&__image_end) - (char *)(&__image_begin);
    /* claim 3MB starting at PROG_START */
    claim(PROG_START, PROG_SIZE, 0);
    dst = (void *) PROG_START;
    if (im[0] == 0x1f && im[1] == 0x8b) {
	/* claim some memory for scratch space */
	avail_ram = (char *) claim(0, SCRATCH_SIZE, 0x10);
	begin_avail = avail_high = avail_ram;
	end_avail = avail_ram + SCRATCH_SIZE;
	printf("heap at 0x%p\n", avail_ram);
	printf("gunzipping (0x%p <- 0x%p:0x%p)...", dst, im, im+len);
	gunzip(dst, PROG_SIZE, im, &len);
	printf("done %u bytes\n", len);
	printf("%u bytes of heap consumed, max in use %u\n",
	       avail_high - begin_avail, heap_max);
	release(begin_avail, SCRATCH_SIZE);
    } else {
	memmove(dst, im, len);
    }

    flush_cache(dst, len);
    make_bi_recs(((unsigned long) dst + len), "chrpboot", _MACH_Pmac,
		    (PROG_START + PROG_SIZE));

    sa = (unsigned long)PROG_START;
    printf("start address = 0x%x\n", sa);

    (*(void (*)())sa)(a1, a2, prom);

    printf("returned?\n");

    pause();
}
