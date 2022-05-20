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
 * linux/include/linux/lockd/share.h
 *
 * DOS share management for lockd.
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_LOCKD_SHARE_H
#define LINUX_LOCKD_SHARE_H

/*
 * DOS share for a specific file
 */
struct nlm_share {
	struct nlm_share *	s_next;		/* linked list */
	struct nlm_host *	s_host;		/* client host */
	struct nlm_file *	s_file;		/* shared file */
	struct xdr_netobj	s_owner;	/* owner handle */
	u32			s_access;	/* access mode */
	u32			s_mode;		/* deny mode */
};

u32	nlmsvc_share_file(struct nlm_host *, struct nlm_file *,
					       struct nlm_args *);
u32	nlmsvc_unshare_file(struct nlm_host *, struct nlm_file *,
					       struct nlm_args *);
int	nlmsvc_traverse_shares(struct nlm_host *, struct nlm_file *, int);

#endif /* LINUX_LOCKD_SHARE_H */
