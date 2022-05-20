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
 * c 2001 PPC 64 Team, IBM Corp
 * 
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>

#include <asm/uaccess.h>
#include <asm/pgalloc.h>

rwlock_t imlist_lock = RW_LOCK_UNLOCKED;
struct vm_struct * imlist = NULL;

struct vm_struct *get_im_area(unsigned long size)
{
	unsigned long addr;
	struct vm_struct **p, *tmp, *area;
  
	area = (struct vm_struct *) kmalloc(sizeof(*area), GFP_KERNEL);
	if (!area)
		return NULL;
	addr = IMALLOC_START;
	write_lock(&imlist_lock);
	for (p = &imlist; (tmp = *p) ; p = &tmp->next) {
		if (size + addr < (unsigned long) tmp->addr)
			break;
		addr = tmp->size + (unsigned long) tmp->addr;
		if (addr > IMALLOC_END-size) {
			write_unlock(&imlist_lock);
			kfree(area);
			return NULL;
		}
	}
	area->flags = 0;
	area->addr = (void *)addr;
	area->size = size;
	area->next = *p;
	*p = area;
	write_unlock(&imlist_lock);
	return area;
}

void ifree(void * addr)
{
	struct vm_struct **p, *tmp;
  
	if (!addr)
		return;
	if ((PAGE_SIZE-1) & (unsigned long) addr) {
		printk(KERN_ERR "Trying to ifree() bad address (%p)\n", addr);
		return;
	}
	write_lock(&imlist_lock);
	for (p = &imlist ; (tmp = *p) ; p = &tmp->next) {
		if (tmp->addr == addr) {
			*p = tmp->next;
			kfree(tmp);
			write_unlock(&imlist_lock);
			return;
		}
	}
	write_unlock(&imlist_lock);
	printk(KERN_ERR "Trying to ifree() nonexistent area (%p)\n", addr);
}

