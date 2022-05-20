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
/* $Id: jffs2_fs_i.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $ */

#ifndef _JFFS2_FS_I
#define _JFFS2_FS_I

/* Include the pipe_inode_info at the beginning so that we can still
   use the storage space in the inode when we have a pipe inode.
   This sucks.
*/

#undef THISSUCKS /* Only for 2.2 */
#ifdef THISSUCKS
#include <linux/pipe_fs_i.h>
#endif

struct jffs2_inode_info {
#ifdef THISSUCKS
        struct pipe_inode_info pipecrap;
#endif
	/* We need an internal semaphore similar to inode->i_sem.
	   Unfortunately, we can't used the existing one, because
	   either the GC would deadlock, or we'd have to release it
	   before letting GC proceed. Or we'd have to put ugliness
	   into the GC code so it didn't attempt to obtain the i_sem
	   for the inode(s) which are already locked */
	struct semaphore sem;

	/* The highest (datanode) version number used for this ino */
	__u32 highest_version;

	/* List of data fragments which make up the file */
	struct jffs2_node_frag *fraglist;

	/* There may be one datanode which isn't referenced by any of the
	   above fragments, if it contains a metadata update but no actual
	   data - or if this is a directory inode */
	/* This also holds the _only_ dnode for symlinks/device nodes, 
	   etc. */
	struct jffs2_full_dnode *metadata;

	/* Directory entries */
	struct jffs2_full_dirent *dents;

	/* Some stuff we just have to keep in-core at all times, for each inode. */
	struct jffs2_inode_cache *inocache;

	/* Keep a pointer to the last physical node in the list. We don't 
	   use the doubly-linked lists because we don't want to increase
	   the memory usage that much. This is simpler */
	//	struct jffs2_raw_node_ref *lastnode;
	__u16 flags;
	__u8 usercompr;
};

#ifdef JFFS2_OUT_OF_KERNEL
#define JFFS2_INODE_INFO(i) ((struct jffs2_inode_info *) &(i)->u)
#else
#define JFFS2_INODE_INFO(i) (&i->u.jffs2_i)
#endif

#endif /* _JFFS2_FS_I */

