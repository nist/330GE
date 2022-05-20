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
 * efs_dir.h
 *
 * Copyright (c) 1999 Al Smith
 */

#ifndef __EFS_DIR_H__
#define __EFS_DIR_H__

#define EFS_DIRBSIZE_BITS	EFS_BLOCKSIZE_BITS
#define EFS_DIRBSIZE		(1 << EFS_DIRBSIZE_BITS)

struct efs_dentry {
	unsigned int	inode;
	unsigned char	namelen;
	char		name[3];
};

#define EFS_DENTSIZE	(sizeof(struct efs_dentry) - 3 + 1)
#define EFS_MAXNAMELEN  ((1 << (sizeof(char) * 8)) - 1)

#define EFS_DIRBLK_HEADERSIZE	4
#define EFS_DIRBLK_MAGIC	0xbeef	/* moo */

struct efs_dir {
	unsigned short	magic;
	unsigned char	firstused;
	unsigned char	slots;

	unsigned char	space[EFS_DIRBSIZE - EFS_DIRBLK_HEADERSIZE];
};

#define EFS_MAXENTS \
	((EFS_DIRBSIZE - EFS_DIRBLK_HEADERSIZE) / \
	 (EFS_DENTSIZE + sizeof(char)))

#define EFS_SLOTAT(dir, slot) EFS_REALOFF((dir)->space[slot])

#define EFS_REALOFF(offset) ((offset << 1))

#endif /* __EFS_DIR_H__ */

