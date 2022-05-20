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
 * include/linux/nfsd/interface.h
 *
 * defines interface between nfsd and other bits of
 * the kernel.  Particularly filesystems (eventually).
 *
 * Copyright (C) 2000 Neil Brown <neilb@cse.unsw.edu.au>
 */

#ifndef LINUX_NFSD_INTERFACE_H
#define LINUX_NFSD_INTERFACE_H

#include <linux/config.h>

#ifndef CONFIG_NFSD
# ifdef CONFIG_MODULES

extern struct nfsd_linkage {
	long (*do_nfsservctl)(int cmd, void *argp, void *resp);
	struct module *owner;
} * nfsd_linkage;

# endif
#endif

#endif /* LINUX_NFSD_INTERFACE_H */
