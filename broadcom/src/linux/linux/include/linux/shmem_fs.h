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
#ifndef __SHMEM_FS_H
#define __SHMEM_FS_H

/* inode in-kernel data */

#define SHMEM_NR_DIRECT 16

/*
 * A swap entry has to fit into a "unsigned long", as
 * the entry is hidden in the "index" field of the
 * swapper address space.
 *
 * We have to move it here, since not every user of fs.h is including
 * mm.h, but mm.h is including fs.h via sched .h :-/
 */
typedef struct {
	unsigned long val;
} swp_entry_t;

extern atomic_t shmem_nrpages;

struct shmem_inode_info {
	spinlock_t		lock;
	struct semaphore 	sem;
	unsigned long		next_index;
	swp_entry_t		i_direct[SHMEM_NR_DIRECT]; /* for the first blocks */
	void		      **i_indirect; /* indirect blocks */
	unsigned long		swapped;
	int			locked;     /* into memory */
	struct list_head	list;
	struct inode	       *inode;
};

struct shmem_sb_info {
	unsigned long max_blocks;   /* How many blocks are allowed */
	unsigned long free_blocks;  /* How many are left for allocation */
	unsigned long max_inodes;   /* How many inodes are allowed */
	unsigned long free_inodes;  /* How many are left for allocation */
	spinlock_t    stat_lock;
};

#define SHMEM_I(inode)  (&inode->u.shmem_i)

#endif
