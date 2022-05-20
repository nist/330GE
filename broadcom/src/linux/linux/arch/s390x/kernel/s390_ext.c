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
 *  arch/s390/kernel/s390_ext.c
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Holger Smolinski (Holger.Smolinski@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/lowcore.h>
#include <asm/s390_ext.h>

/*
 * Simple hash strategy: index = code & 0xff;
 * ext_int_hash[index] is the start of the list for all external interrupts
 * that hash to this index. With the current set of external interrupts 
 * (0x1202 external call, 0x1004 cpu timer, 0x2401 hwc console, 0x4000
 * iucv and 0x2603 pfault) this is always the first element. 
 */
ext_int_info_t *ext_int_hash[256] = { 0, };

int register_external_interrupt(__u16 code, ext_int_handler_t handler) {
        ext_int_info_t *p;
        int index;

	p = (ext_int_info_t *) kmalloc(sizeof(ext_int_info_t), GFP_ATOMIC);
        if (p == NULL)
                return -ENOMEM;
        p->code = code;
        p->handler = handler;
        index = code & 0xff;
        p->next = ext_int_hash[index];
        ext_int_hash[index] = p;
        return 0;
}

int register_early_external_interrupt(__u16 code, ext_int_handler_t handler,
				      ext_int_info_t *p) {
        int index;

        if (p == NULL)
                return -EINVAL;
        p->code = code;
        p->handler = handler;
        index = code & 0xff;
        p->next = ext_int_hash[index];
        ext_int_hash[index] = p;
        return 0;
}

int unregister_external_interrupt(__u16 code, ext_int_handler_t handler) {
        ext_int_info_t *p, *q;
        int index;

        index = code & 0xff;
        q = NULL;
        p = ext_int_hash[index];
        while (p != NULL) {
                if (p->code == code && p->handler == handler)
                        break;
                q = p;
                p = p->next;
        }
        if (p == NULL)
                return -ENOENT;
        if (q != NULL)
                q->next = p->next;
        else
                ext_int_hash[index] = p->next;
	kfree(p);
        return 0;
}

int unregister_early_external_interrupt(__u16 code, ext_int_handler_t handler,
					ext_int_info_t *p) {
	ext_int_info_t *q;
	int index;

	if (p == NULL || p->code != code || p->handler != handler)
		return -EINVAL;
	index = code & 0xff;
	q = ext_int_hash[index];
	if (p != q) {
		while (q != NULL) {
			if (q->next == p)
				break;
			q = q->next;
		}
		if (q == NULL)
			return -ENOENT;
		q->next = p->next;
	} else
		ext_int_hash[index] = p->next;
	return 0;
}

EXPORT_SYMBOL(register_external_interrupt);
EXPORT_SYMBOL(unregister_external_interrupt);

