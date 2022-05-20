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
/* -*- c -*-
 * linux/include/linux/auto_fs4.h
 *
 * Copyright 1999-2000 Jeremy Fitzhardinge <jeremy@goop.org>
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 */

#ifndef _LINUX_AUTO_FS4_H
#define _LINUX_AUTO_FS4_H

/* Include common v3 definitions */
#include <linux/auto_fs.h>

/* autofs v4 definitions */
#undef AUTOFS_PROTO_VERSION
#undef AUTOFS_MIN_PROTO_VERSION
#undef AUTOFS_MAX_PROTO_VERSION

#define AUTOFS_PROTO_VERSION		4
#define AUTOFS_MIN_PROTO_VERSION	3
#define AUTOFS_MAX_PROTO_VERSION	4

/* New message type */
#define autofs_ptype_expire_multi	2	/* Expire entry (umount request) */

/* v4 multi expire (via pipe) */
struct autofs_packet_expire_multi {
	struct autofs_packet_hdr hdr;
        autofs_wqt_t wait_queue_token;
	int len;
	char name[NAME_MAX+1];
};

union autofs_packet_union {
	struct autofs_packet_hdr hdr;
	struct autofs_packet_missing missing;
	struct autofs_packet_expire expire;
	struct autofs_packet_expire_multi expire_multi;
};

#define AUTOFS_IOC_EXPIRE_MULTI _IOW(0x93,0x66,int)


#endif /* _LINUX_AUTO_FS4_H */
