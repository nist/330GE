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
**  linux/amiga/chipram.c
**
**      Modified 03-May-94 by Geert Uytterhoeven <geert@linux-m68k.org>
**          - 64-bit aligned allocations for full AGA compatibility
**
**	Rewritten 15/9/2000 by Geert to use resource management
*/

#include <linux/config.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <asm/amigahw.h>

unsigned long amiga_chip_size;

static struct resource chipram_res = { "Chip RAM", CHIP_PHYSADDR };
static unsigned long chipavail;


void __init amiga_chip_init(void)
{
    if (!AMIGAHW_PRESENT(CHIP_RAM))
	return;

#ifndef CONFIG_APUS_FAST_EXCEPT
    /*
     *  Remove the first 4 pages where PPC exception handlers will be located
     */
    amiga_chip_size -= 0x4000;
#endif
    chipram_res.end = amiga_chip_size-1;
    request_resource(&iomem_resource, &chipram_res);

    chipavail = amiga_chip_size;
}

    
void *amiga_chip_alloc(unsigned long size, const char *name)
{
    struct resource *res;

    /* round up */
    size = PAGE_ALIGN(size);

#ifdef DEBUG
    printk("amiga_chip_alloc: allocate %ld bytes\n", size);
#endif
    res = kmalloc(sizeof(struct resource), GFP_KERNEL);
    if (!res)
	return NULL;
    memset(res, 0, sizeof(struct resource));
    res->name = name;

    if (allocate_resource(&chipram_res, res, size, 0, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0) {
	kfree(res);
	return NULL;
    }
    chipavail -= size;
#ifdef DEBUG
    printk("amiga_chip_alloc: returning %lx\n", res->start);
#endif
    return (void *)ZTWO_VADDR(res->start);
}


    /*
     *  Warning:
     *  amiga_chip_alloc_res is meant only for drivers that need to allocate
     *  Chip RAM before kmalloc() is functional. As a consequence, those
     *  drivers must not free that Chip RAM afterwards.
     */

void * __init amiga_chip_alloc_res(unsigned long size, struct resource *res)
{
    unsigned long start;

    /* round up */
    size = PAGE_ALIGN(size);
    /* dmesg into chipmem prefers memory at the safe end */
    start = CHIP_PHYSADDR + chipavail - size;

#ifdef DEBUG
    printk("amiga_chip_alloc_res: allocate %ld bytes\n", size);
#endif
    if (allocate_resource(&chipram_res, res, size, start, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0) {
	printk("amiga_chip_alloc_res: first alloc failed!\n");
	if (allocate_resource(&chipram_res, res, size, 0, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0)
	    return NULL;
    }
    chipavail -= size;
#ifdef DEBUG
    printk("amiga_chip_alloc_res: returning %lx\n", res->start);
#endif
    return (void *)ZTWO_VADDR(res->start);
}

void amiga_chip_free(void *ptr)
{
    unsigned long start = ZTWO_PADDR(ptr);
    struct resource **p, *res;
    unsigned long size;

    for (p = &chipram_res.child; (res = *p); p = &res->sibling) {
	if (res->start != start)
	    continue;
	*p = res->sibling;
	size = res->end-start;
#ifdef DEBUG
	printk("amiga_chip_free: free %ld bytes at %p\n", size, ptr);
#endif
	chipavail += size;
	kfree(res);
	return;
    }
    printk("amiga_chip_free: trying to free nonexistent region at %p\n", ptr);
}


unsigned long amiga_chip_avail(void)
{
#ifdef DEBUG
	printk("amiga_chip_avail : %ld bytes\n", chipavail);
#endif
	return chipavail;
}
