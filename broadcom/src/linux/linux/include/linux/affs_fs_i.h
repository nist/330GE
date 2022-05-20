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
#ifndef _AFFS_FS_I
#define _AFFS_FS_I

#include <linux/a.out.h>

// move this to linux/coda.h!!!
#include <linux/time.h>

#define AFFS_CACHE_SIZE		PAGE_SIZE
//#define AFFS_CACHE_SIZE		(4*4)

#define AFFS_MAX_PREALLOC	32
#define AFFS_LC_SIZE		(AFFS_CACHE_SIZE/sizeof(u32)/2)
#define AFFS_AC_SIZE		(AFFS_CACHE_SIZE/sizeof(struct affs_ext_key)/2)
#define AFFS_AC_MASK		(AFFS_AC_SIZE-1)

struct affs_ext_key {
	u32	ext;				/* idx of the extended block */
	u32	key;				/* block number */
};

/*
 * affs fs inode data in memory
 */
struct affs_inode_info {
	u32	 i_opencnt;
	struct semaphore i_link_lock;		/* Protects internal inode access. */
	struct semaphore i_ext_lock;		/* Protects internal inode access. */
#define i_hash_lock i_ext_lock
	u32	 i_blkcnt;			/* block count */
	u32	 i_extcnt;			/* extended block count */
	u32	*i_lc;				/* linear cache of extended blocks */
	u32	 i_lc_size;
	u32	 i_lc_shift;
	u32	 i_lc_mask;
	struct affs_ext_key *i_ac;		/* associative cache of extended blocks */
	u32	 i_ext_last;			/* last accessed extended block */
	struct buffer_head *i_ext_bh;		/* bh of last extended block */
	unsigned long mmu_private;
	u32	 i_protect;			/* unused attribute bits */
	u32	 i_lastalloc;			/* last allocated block */
	int	 i_pa_cnt;			/* number of preallocated blocks */
};

/* short cut to get to the affs specific inode data */
#define AFFS_INODE	(&inode->u.affs_i)
#define AFFS_DIR	(&dir->u.affs_i)

#endif
