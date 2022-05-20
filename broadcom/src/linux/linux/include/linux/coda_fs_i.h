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
 *  coda_fs_i.h
 *
 *  Copyright (C) 1998 Carnegie Mellon University
 *
 */

#ifndef _LINUX_CODA_FS_I
#define _LINUX_CODA_FS_I

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/list.h>
#include <linux/coda.h>

/*
 * coda fs inode data
 */
struct coda_inode_info {
        struct ViceFid     c_fid;	/* Coda identifier */
        u_short	           c_flags;     /* flags (see below) */
	struct list_head   c_cilist;    /* list of all coda inodes */
	int		   c_mapcount;	/* how often is this inode mmapped */
        struct coda_cred   c_cached_cred; /* credentials of cached perms */
        unsigned int       c_cached_perm; /* cached access permissions */
};

/*
 * coda fs file private data
 */
#define CODA_MAGIC 0xC0DAC0DA
struct coda_file_info {
	int		 cfi_magic;	/* magic number */
	int		 cfi_mapcount;  /* how often this file is mapped */
	struct file	*cfi_container;	/* container file for this cnode */
	struct coda_cred cfi_cred;	/* credentials of opener */
};

#define CODA_FTOC(file) ((struct coda_file_info *)((file)->private_data))

/* flags */
#define C_VATTR       0x1   /* Validity of vattr in inode */
#define C_FLUSH       0x2   /* used after a flush */
#define C_DYING       0x4   /* from venus (which died) */
#define C_PURGE       0x8

int coda_cnode_make(struct inode **, struct ViceFid *, struct super_block *);
struct inode *coda_iget(struct super_block *sb, struct ViceFid *fid, struct coda_vattr *attr);
int coda_cnode_makectl(struct inode **inode, struct super_block *sb);
struct inode *coda_fid_to_inode(ViceFid *fid, struct super_block *sb);
void coda_replace_fid(struct inode *, ViceFid *, ViceFid *);

#endif
#endif