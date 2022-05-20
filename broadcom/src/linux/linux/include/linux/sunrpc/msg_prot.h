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
 * linux/include/net/sunrpc/msg_prot.h
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_MSGPROT_H_
#define _LINUX_SUNRPC_MSGPROT_H_

#ifdef __KERNEL__     /* user programs should get these from the rpc header files */

#define RPC_VERSION 2

enum rpc_auth_flavor {
	RPC_AUTH_NULL  = 0,
	RPC_AUTH_UNIX  = 1,
	RPC_AUTH_SHORT = 2,
	RPC_AUTH_DES   = 3,
	RPC_AUTH_KRB   = 4,
};

enum rpc_msg_type {
	RPC_CALL = 0,
	RPC_REPLY = 1
};

enum rpc_reply_stat {
	RPC_MSG_ACCEPTED = 0,
	RPC_MSG_DENIED = 1
};

enum rpc_accept_stat {
	RPC_SUCCESS = 0,
	RPC_PROG_UNAVAIL = 1,
	RPC_PROG_MISMATCH = 2,
	RPC_PROC_UNAVAIL = 3,
	RPC_GARBAGE_ARGS = 4,
	RPC_SYSTEM_ERR = 5
};

enum rpc_reject_stat {
	RPC_MISMATCH = 0,
	RPC_AUTH_ERROR = 1
};

enum rpc_auth_stat {
	RPC_AUTH_OK = 0,
	RPC_AUTH_BADCRED = 1,
	RPC_AUTH_REJECTEDCRED = 2,
	RPC_AUTH_BADVERF = 3,
	RPC_AUTH_REJECTEDVERF = 4,
	RPC_AUTH_TOOWEAK = 5
};

#define RPC_PMAP_PROGRAM	100000
#define RPC_PMAP_VERSION	2
#define RPC_PMAP_PORT		111

#define RPC_MAXNETNAMELEN	256

#endif /* __KERNEL__ */
#endif /* _LINUX_SUNRPC_MSGPROT_H_ */
