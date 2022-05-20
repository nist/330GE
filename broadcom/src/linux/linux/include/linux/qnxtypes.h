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
 *  Name                         : qnxtypes.h
 *  Author                       : Richard Frowijn
 *  Function                     : standard qnx types
 *  Version                      : 1.0.2
 *  Last modified                : 2000-01-06
 *
 *  History                      : 22-03-1998 created
 *
 */

#ifndef _QNX4TYPES_H
#define _QNX4TYPES_H

typedef __u16 qnx4_nxtnt_t;
typedef __u8  qnx4_ftype_t;

typedef struct {
	__u32 xtnt_blk;
	__u32 xtnt_size;
} qnx4_xtnt_t;

typedef __u16 qnx4_mode_t;
typedef __u16 qnx4_muid_t;
typedef __u16 qnx4_mgid_t;
typedef __u32 qnx4_off_t;
typedef __u16 qnx4_nlink_t;

#endif
