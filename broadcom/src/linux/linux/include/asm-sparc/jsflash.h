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
 * jsflash.h: OS Flash SIMM support for JavaStations.
 *
 * Copyright (C) 1999  Pete Zaitcev
 */

#ifndef _SPARC_JSFLASH_H
#define _SPARC_JSFLASH_H

#ifndef _SPARC_TYPES_H
#include <asm/types.h>
#endif

/*
 * Semantics of the offset is a full address.
 * Hardcode it or get it from probe ioctl.
 *
 * We use full bus address, so that we would be
 * automatically compatible with possible future systems.
 */

#define JSFLASH_IDENT   (('F'<<8)|54)
struct jsflash_ident_arg {
	__u64 off;                /* 0x20000000 is included */
	__u32 size;
	char name[32];		/* With trailing zero */
};

#define JSFLASH_ERASE   (('F'<<8)|55)
/* Put 0 as argument, may be flags or sector number... */

#define JSFLASH_PROGRAM (('F'<<8)|56)
struct jsflash_program_arg {
	__u64 data;		/* char* for sparc and sparc64 */
	__u64 off;
	__u32 size;
};

#endif /* _SPARC_JSFLASH_H */
