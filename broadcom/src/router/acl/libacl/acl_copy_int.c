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
  File: acl_copy_int.c

  Copyright (C) 1999, 2000
  Andreas Gruenbacher, <a.gruenbacher@computer.org>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "libacl.h"


/* 23.4.6 */
acl_t
acl_copy_int(const void *buf_p)
{
	const struct __acl *ext_acl = (struct __acl *)buf_p;
	const struct __acl_entry *ent_p = ext_acl->x_entries, *end_p;
	size_t size = ext_acl ? ext_acl->x_size : 0;
	int entries;
	acl_obj *acl_obj_p;
	acl_entry_obj *entry_obj_p;

	if (!ext_acl || size < sizeof(struct __acl)) {
		errno = EINVAL;
		return NULL;
	}
	size -= sizeof(struct __acl);
	if (size % sizeof(struct __acl_entry)) {
		errno = EINVAL;
		return NULL;
	}
	entries = size / sizeof(struct __acl_entry);
	acl_obj_p = __acl_init_obj(entries);
	if (acl_obj_p == NULL)
		goto fail;
	end_p = ext_acl->x_entries + entries;
	for(; ent_p != end_p; ent_p++) {
		entry_obj_p = __acl_create_entry_obj(acl_obj_p);
		if (!entry_obj_p)
			goto fail;
		/* XXX Convert to machine endianness */
		entry_obj_p->eentry = *ent_p;
	}
	if (__acl_reorder_obj_p(acl_obj_p))
		goto fail;
	return int2ext(acl_obj_p);

fail:
	__acl_free_acl_obj(acl_obj_p);
	return NULL;
}

