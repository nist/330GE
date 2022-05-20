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
 * linux/include/linux/sunrpc/svcauth.h
 *
 * RPC server-side authentication stuff.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_SVCAUTH_H_
#define _LINUX_SUNRPC_SVCAUTH_H_

#ifdef __KERNEL__

#include <linux/sunrpc/msg_prot.h>

struct svc_cred {
	u32			cr_flavor;
	uid_t			cr_uid;
	gid_t			cr_gid;
	gid_t			cr_groups[NGROUPS];
};

struct svc_rqst;		/* forward decl */

void	svc_authenticate(struct svc_rqst *rqstp, u32 *statp, u32 *authp);
int	svc_auth_register(u32 flavor, void (*)(struct svc_rqst *,u32 *,u32 *));
void	svc_auth_unregister(u32 flavor);



#endif /* __KERNEL__ */

#endif /* _LINUX_SUNRPC_SVCAUTH_H_ */
