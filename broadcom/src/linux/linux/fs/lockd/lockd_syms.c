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
 * linux/fs/lockd/lockd_syms.c
 *
 * Symbols exported by the lockd module.
 *
 * Authors:	Olaf Kirch (okir@monad.swb.de)
 *
 * Copyright (C) 1997 Olaf Kirch <okir@monad.swb.de>
 */

#define __NO_VERSION__
#include <linux/config.h>
#include <linux/module.h>

#ifdef CONFIG_MODULES

#include <linux/types.h>
#include <linux/socket.h>
#include <linux/sched.h>
#include <linux/uio.h>
#include <linux/unistd.h>

#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/svc.h>
#include <linux/lockd/lockd.h>

/* Start/stop the daemon */
EXPORT_SYMBOL(lockd_up);
EXPORT_SYMBOL(lockd_down);

/* NFS client entry */
EXPORT_SYMBOL(nlmclnt_proc);

/* NFS server entry points/hooks */
EXPORT_SYMBOL(nlmsvc_invalidate_client);
EXPORT_SYMBOL(nlmsvc_ops);

#endif /* CONFIG_MODULES */
