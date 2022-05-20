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
 *	include/linux/bfs_fs.h - BFS data structures on disk.
 *	Copyright (C) 1999 Tigran Aivazian <tigran@veritas.com>
 */

#ifndef _LINUX_BFS_FS_H
#define _LINUX_BFS_FS_H

#define BFS_BSIZE_BITS		9
#define BFS_BSIZE		(1<<BFS_BSIZE_BITS)

#define BFS_MAGIC		0x1BADFACE
#define BFS_ROOT_INO		2
#define BFS_INODES_PER_BLOCK	8

/* SVR4 vnode type values (bfs_inode->i_vtype) */
#define BFS_VDIR		2
#define BFS_VREG		1

/* BFS inode layout on disk */
struct bfs_inode {
	__u16 i_ino;
	__u16 i_unused;
	__u32 i_sblock;
	__u32 i_eblock;
	__u32 i_eoffset;
	__u32 i_vtype;
	__u32 i_mode;
	__s32 i_uid;
	__s32 i_gid;
	__u32 i_nlink;
	__u32 i_atime;
	__u32 i_mtime;
	__u32 i_ctime;
	__u32 i_padding[4];
};

#define BFS_NAMELEN		14	
#define BFS_DIRENT_SIZE		16
#define BFS_DIRS_PER_BLOCK	32

struct bfs_dirent {
	__u16 ino;
	char name[BFS_NAMELEN];
};

/* BFS superblock layout on disk */
struct bfs_super_block {
	__u32 s_magic;
	__u32 s_start;
	__u32 s_end;
	__s32 s_from;
	__s32 s_to;
	__s32 s_bfrom;
	__s32 s_bto;
	char  s_fsname[6];
	char  s_volume[6];
	__u32 s_padding[118];
};

#define BFS_NZFILESIZE(ip) \
        (((ip)->i_eoffset + 1) - (ip)->i_sblock * BFS_BSIZE)

#define BFS_FILESIZE(ip) \
        ((ip)->i_sblock == 0 ? 0 : BFS_NZFILESIZE(ip))

#define BFS_FILEBLOCKS(ip) \
        ((ip)->i_sblock == 0 ? 0 : ((ip)->i_eblock + 1) - (ip)->i_sblock)

#define BFS_OFF2INO(offset) \
        ((((offset) - BFS_BSIZE) / sizeof(struct bfs_inode)) + BFS_ROOT_INO)

#define BFS_INO2OFF(ino) \
	((__u32)(((ino) - BFS_ROOT_INO) * sizeof(struct bfs_inode)) + BFS_BSIZE)

#define BFS_UNCLEAN(bfs_sb, sb)	\
	((bfs_sb->s_from != -1) && (bfs_sb->s_to != -1) && !(sb->s_flags & MS_RDONLY))

#ifdef __KERNEL__

/* file.c */
extern struct inode_operations bfs_file_inops;
extern struct file_operations bfs_file_operations;
extern struct address_space_operations bfs_aops;

/* dir.c */
extern struct inode_operations bfs_dir_inops;
extern struct file_operations bfs_dir_operations;

#endif /* __KERNEL__ */
#endif	/* _LINUX_BFS_FS_H */
