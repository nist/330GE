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
 * linux/include/linux/sunrpc/types.h
 *
 * Generic types and misc stuff for RPC.
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_TYPES_H_
#define _LINUX_SUNRPC_TYPES_H_

#include <linux/timer.h>
#include <linux/tqueue.h>
#include <linux/sunrpc/debug.h>
#include <linux/list.h>

/*
 * Shorthands
 */
#define signalled()		(signal_pending(current))

#endif /* _LINUX_SUNRPC_TYPES_H_ */
