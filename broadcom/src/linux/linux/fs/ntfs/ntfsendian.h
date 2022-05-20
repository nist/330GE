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
 * ntfsendian.h
 *
 * Copyright (C) 1998, 1999 Martin von Löwis
 * Copyright (C) 1998 Joseph Malicki
 * Copyright (C) 1999 Werner Seiler
 * Copyright (C) 2001 Anton Altaparmakov (AIA)
 */
#include <asm/byteorder.h>

#define CPU_TO_LE16(a) __cpu_to_le16(a)
#define CPU_TO_LE32(a) __cpu_to_le32(a)
#define CPU_TO_LE64(a) __cpu_to_le64(a)

#define LE16_TO_CPU(a) __cpu_to_le16(a)
#define LE32_TO_CPU(a) __cpu_to_le32(a)
#define LE64_TO_CPU(a) __cpu_to_le64(a)

#define NTFS_GETU8(p)      (*(ntfs_u8*)(p))
#define NTFS_GETU16(p)     ((ntfs_u16)LE16_TO_CPU(*(ntfs_u16*)(p)))
#define NTFS_GETU24(p)     ((ntfs_u32)NTFS_GETU16(p) | \
			   ((ntfs_u32)NTFS_GETU8(((char*)(p)) + 2) << 16))
#define NTFS_GETU32(p)     ((ntfs_u32)LE32_TO_CPU(*(ntfs_u32*)(p)))
#define NTFS_GETU40(p)     ((ntfs_u64)NTFS_GETU32(p) | \
			   (((ntfs_u64)NTFS_GETU8(((char*)(p)) + 4)) << 32))
#define NTFS_GETU48(p)     ((ntfs_u64)NTFS_GETU32(p) | \
			   (((ntfs_u64)NTFS_GETU16(((char*)(p)) + 4)) << 32))
#define NTFS_GETU56(p)     ((ntfs_u64)NTFS_GETU32(p) | \
			   (((ntfs_u64)NTFS_GETU24(((char*)(p)) + 4)) << 32))
#define NTFS_GETU64(p)     ((ntfs_u64)LE64_TO_CPU(*(ntfs_u64*)(p)))
 
 /* Macros writing unsigned integers */
#define NTFS_PUTU8(p,v)      ((*(ntfs_u8*)(p)) = (v))
#define NTFS_PUTU16(p,v)     ((*(ntfs_u16*)(p)) = CPU_TO_LE16(v))
#define NTFS_PUTU24(p,v)     NTFS_PUTU16(p, (v) & 0xFFFF);\
                             NTFS_PUTU8(((char*)(p)) + 2, (v) >> 16)
#define NTFS_PUTU32(p,v)     ((*(ntfs_u32*)(p)) = CPU_TO_LE32(v))
#define NTFS_PUTU64(p,v)     ((*(ntfs_u64*)(p)) = CPU_TO_LE64(v))
 
 /* Macros reading signed integers */
#define NTFS_GETS8(p)        ((*(ntfs_s8*)(p)))
#define NTFS_GETS16(p)       ((ntfs_s16)LE16_TO_CPU(*(short*)(p)))
#define NTFS_GETS24(p)       (NTFS_GETU24(p) < 0x800000 ? \
					(int)NTFS_GETU24(p) : \
					(int)(NTFS_GETU24(p) - 0x1000000))
#define NTFS_GETS32(p)       ((ntfs_s32)LE32_TO_CPU(*(int*)(p)))
#define NTFS_GETS40(p)       (((ntfs_s64)NTFS_GETU32(p)) | \
			     (((ntfs_s64)NTFS_GETS8(((char*)(p)) + 4)) << 32))
#define NTFS_GETS48(p)       (((ntfs_s64)NTFS_GETU32(p)) | \
			     (((ntfs_s64)NTFS_GETS16(((char*)(p)) + 4)) << 32))
#define NTFS_GETS56(p)       (((ntfs_s64)NTFS_GETU32(p)) | \
			     (((ntfs_s64)NTFS_GETS24(((char*)(p)) + 4)) << 32))
#define NTFS_GETS64(p)	     ((ntfs_s64)NTFS_GETU64(p))
 
#define NTFS_PUTS8(p,v)      NTFS_PUTU8(p,v)
#define NTFS_PUTS16(p,v)     NTFS_PUTU16(p,v)
#define NTFS_PUTS24(p,v)     NTFS_PUTU24(p,v)
#define NTFS_PUTS32(p,v)     NTFS_PUTU32(p,v)
#define NTFS_PUTS64(p,v)     NTFS_PUTU64(p,v)

