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
#ifndef __LINUX_VMALLOC_H
#define __LINUX_VMALLOC_H

#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/spinlock.h>

#include <linux/highmem.h>	/* several arch define VMALLOC_END via PKMAP_BASE */
#include <asm/pgtable.h>

/* bits in vm_struct->flags */
#define VM_IOREMAP	0x00000001	/* ioremap() and friends */
#define VM_ALLOC	0x00000002	/* vmalloc() */

struct vm_struct {
	unsigned long flags;
	void * addr;
	unsigned long size;
	struct vm_struct * next;
};

extern struct vm_struct * get_vm_area (unsigned long size, unsigned long flags);
extern void vfree(void * addr);
extern void * __vmalloc (unsigned long size, int gfp_mask, pgprot_t prot);
extern long vread(char *buf, char *addr, unsigned long count);
extern void vmfree_area_pages(unsigned long address, unsigned long size);
extern int vmalloc_area_pages(unsigned long address, unsigned long size,
                              int gfp_mask, pgprot_t prot);

/*
 *	Allocate any pages
 */
 
static inline void * vmalloc (unsigned long size)
{
	return __vmalloc(size, GFP_KERNEL | __GFP_HIGHMEM, PAGE_KERNEL);
}

/*
 *	Allocate ISA addressable pages for broke crap
 */

static inline void * vmalloc_dma (unsigned long size)
{
	return __vmalloc(size, GFP_KERNEL|GFP_DMA, PAGE_KERNEL);
}

/*
 *	vmalloc 32bit PA addressable pages - eg for PCI 32bit devices
 */
 
static inline void * vmalloc_32(unsigned long size)
{
	return __vmalloc(size, GFP_KERNEL, PAGE_KERNEL);
}

/*
 * vmlist_lock is a read-write spinlock that protects vmlist
 * Used in mm/vmalloc.c (get_vm_area() and vfree()) and fs/proc/kcore.c.
 */
extern rwlock_t vmlist_lock;

extern struct vm_struct * vmlist;
#endif

