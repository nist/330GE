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
 *  arch/s390/mm/extable.c
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Hartmut Penner (hp@de.ibm.com)
 *
 *  Derived from "arch/i386/mm/extable.c"
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>

extern const struct exception_table_entry __start___ex_table[];
extern const struct exception_table_entry __stop___ex_table[];

static inline unsigned long
search_one_table(const struct exception_table_entry *first,
		 const struct exception_table_entry *last,
		 unsigned long value)
{
        while (first <= last) {
		const struct exception_table_entry *mid;
		long diff;

		mid = (last - first) / 2 + first;
		diff = mid->insn - value;
                if (diff == 0)
                        return mid->fixup;
                else if (diff < 0)
                        first = mid+1;
                else
                        last = mid-1;
        }
        return 0;
}

extern spinlock_t modlist_lock;

unsigned long
search_exception_table(unsigned long addr)
{
	unsigned long ret = 0;
	unsigned long flags;

#ifndef CONFIG_MODULES
        addr &= 0x7fffffff;  /* remove amode bit from address */
	/* There is only the kernel to search.  */
	ret = search_one_table(__start___ex_table, __stop___ex_table-1, addr);
	if (ret) ret = FIX_PSW(ret);
	return ret;
#else
	/* The kernel is the last "module" -- no need to treat it special.  */
	struct module *mp;
        addr &= 0x7fffffff;  /* remove amode bit from address */

	spin_lock_irqsave(&modlist_lock, flags);
	for (mp = module_list; mp != NULL; mp = mp->next) {
		if (mp->ex_table_start == NULL || !(mp->flags&(MOD_RUNNING|MOD_INITIALIZING)))
			continue;
		ret = search_one_table(mp->ex_table_start,
				       mp->ex_table_end - 1, addr);
		if (ret) {
			ret = FIX_PSW(ret);
			break;
		}
	}
	spin_unlock_irqrestore(&modlist_lock, flags);
	return ret;
#endif
}
