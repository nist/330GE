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
 *  linux/fs/ufs/swab.h
 *
 * Copyright (C) 1997, 1998 Francois-Rene Rideau <fare@tunes.org>
 * Copyright (C) 1998 Jakub Jelinek <jj@ultra.linux.cz>
 * Copyright (C) 2001 Christoph Hellwig <hch@infradead.org>
 */

#ifndef _UFS_SWAB_H
#define _UFS_SWAB_H

/*
 * Notes:
 *    HERE WE ASSUME EITHER BIG OR LITTLE ENDIAN UFSes
 *    in case there are ufs implementations that have strange bytesexes,
 *    you'll need to modify code here as well as in ufs_super.c and ufs_fs.h
 *    to support them.
 */

enum {
	BYTESEX_LE,
	BYTESEX_BE
};

static __inline u64
fs64_to_cpu(struct super_block *sbp, u64 n)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return le64_to_cpu(n);
	else
		return be64_to_cpu(n);
}

static __inline u64
cpu_to_fs64(struct super_block *sbp, u64 n)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return cpu_to_le64(n);
	else
		return cpu_to_be64(n);
}

static __inline u32
fs64_add(struct super_block *sbp, u32 *n, int d)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return *n = cpu_to_le64(le64_to_cpu(*n)+d);
	else
		return *n = cpu_to_be64(be64_to_cpu(*n)+d);
}

static __inline u32
fs64_sub(struct super_block *sbp, u32 *n, int d)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return *n = cpu_to_le64(le64_to_cpu(*n)-d);
	else
		return *n = cpu_to_be64(be64_to_cpu(*n)-d);
}

static __inline u32
fs32_to_cpu(struct super_block *sbp, u32 n)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return le32_to_cpu(n);
	else
		return be32_to_cpu(n);
}

static __inline u32
cpu_to_fs32(struct super_block *sbp, u32 n)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return cpu_to_le32(n);
	else
		return cpu_to_be32(n);
}

static __inline u32
fs32_add(struct super_block *sbp, u32 *n, int d)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return *n = cpu_to_le32(le32_to_cpu(*n)+d);
	else
		return *n = cpu_to_be32(be32_to_cpu(*n)+d);
}

static __inline u32
fs32_sub(struct super_block *sbp, u32 *n, int d)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return *n = cpu_to_le32(le32_to_cpu(*n)-d);
	else
		return *n = cpu_to_be32(be32_to_cpu(*n)-d);
}

static __inline u16
fs16_to_cpu(struct super_block *sbp, u16 n)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return le16_to_cpu(n);
	else
		return be16_to_cpu(n);
}

static __inline u16
cpu_to_fs16(struct super_block *sbp, u16 n)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return cpu_to_le16(n);
	else
		return cpu_to_be16(n);
}

static __inline u16
fs16_add(struct super_block *sbp, u16 *n, int d)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return *n = cpu_to_le16(le16_to_cpu(*n)+d);
	else
		return *n = cpu_to_be16(be16_to_cpu(*n)+d);
}

static __inline u16
fs16_sub(struct super_block *sbp, u16 *n, int d)
{
	if (sbp->u.ufs_sb.s_bytesex == BYTESEX_LE)
		return *n = cpu_to_le16(le16_to_cpu(*n)-d);
	else
		return *n = cpu_to_be16(be16_to_cpu(*n)-d);
}

#endif /* _UFS_SWAB_H */
