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
/*  $Id: modutil.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 *  arch/sparc64/mm/modutil.c
 *
 *  Copyright (C) 1997,1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 *  Based upon code written by Linus Torvalds and others.
 */
 
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <asm/uaccess.h>
#include <asm/system.h>

static struct vm_struct * modvmlist = NULL;

void module_unmap (void * addr)
{
	struct vm_struct **p, *tmp;

	if (!addr)
		return;
	if ((PAGE_SIZE-1) & (unsigned long) addr) {
		printk("Trying to unmap module with bad address (%p)\n", addr);
		return;
	}
	for (p = &modvmlist ; (tmp = *p) ; p = &tmp->next) {
		if (tmp->addr == addr) {
			*p = tmp->next;
			vmfree_area_pages(VMALLOC_VMADDR(tmp->addr), tmp->size);
			kfree(tmp);
			return;
		}
	}
	printk("Trying to unmap nonexistent module vm area (%p)\n", addr);
}

void * module_map (unsigned long size)
{
	void * addr;
	struct vm_struct **p, *tmp, *area;

	size = PAGE_ALIGN(size);
	if (!size || size > MODULES_LEN) return NULL;
		
	addr = (void *) MODULES_VADDR;
	for (p = &modvmlist; (tmp = *p) ; p = &tmp->next) {
		if (size + (unsigned long) addr < (unsigned long) tmp->addr)
			break;
		addr = (void *) (tmp->size + (unsigned long) tmp->addr);
	}
	if ((unsigned long) addr + size >= MODULES_END) return NULL;
	
	area = (struct vm_struct *) kmalloc(sizeof(*area), GFP_KERNEL);
	if (!area) return NULL;
	area->size = size + PAGE_SIZE;
	area->addr = addr;
	area->next = *p;
	*p = area;

	if (vmalloc_area_pages(VMALLOC_VMADDR(addr), size, GFP_KERNEL, PAGE_KERNEL)) {
		module_unmap(addr);
		return NULL;
	}
	return addr;
}
