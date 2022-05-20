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
  File: acl_get_qualifier.c

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


/* 23.4.18 */
void *
acl_get_qualifier(acl_entry_t entry_d)
{
	acl_entry_obj *entry_obj_p = ext2int(acl_entry, entry_d);
	qualifier_obj *qualifier_obj_p;
	if (!entry_obj_p)
		return NULL;

	switch(entry_obj_p->etag) {
		case ACL_USER:
		case ACL_GROUP:
			break;
		default:
			errno = EINVAL;
			return NULL;
	}
	qualifier_obj_p = new_obj_p(qualifier);
	if (!qualifier_obj_p)
		return NULL;
	qualifier_obj_p->qid = entry_obj_p->eid.qid;
	return int2ext(qualifier_obj_p);
}

