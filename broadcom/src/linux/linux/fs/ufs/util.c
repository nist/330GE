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
 *  linux/fs/ufs/util.c
 *
 * Copyright (C) 1998
 * Daniel Pirkl <daniel.pirkl@email.cz>
 * Charles University, Faculty of Mathematics and Physics
 */
 
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/locks.h>

#include "swab.h"
#include "util.h"

#undef UFS_UTILS_DEBUG

#ifdef UFS_UTILS_DEBUG
#define UFSD(x) printk("(%s, %d), %s: ", __FILE__, __LINE__, __FUNCTION__); printk x;
#else
#define UFSD(x)
#endif


struct ufs_buffer_head * _ubh_bread_ (struct ufs_sb_private_info * uspi,
	struct super_block *sb, unsigned fragment, unsigned size)
{
	struct ufs_buffer_head * ubh;
	unsigned i, j, count;
	if (size & ~uspi->s_fmask)
		return NULL;
	count = size >> uspi->s_fshift;
	if (count > UFS_MAXFRAG)
		return NULL;
	ubh = (struct ufs_buffer_head *)
		kmalloc (sizeof (struct ufs_buffer_head), GFP_KERNEL);
	if (!ubh)
		return NULL;
	ubh->fragment = fragment;
	ubh->count = count;
	for (i = 0; i < count; i++)
		if (!(ubh->bh[i] = sb_bread(sb, fragment + i)))
			goto failed;
	for (; i < UFS_MAXFRAG; i++)
		ubh->bh[i] = NULL;
	return ubh;
failed:
	for (j = 0; j < i; j++)
		brelse (ubh->bh[j]);
	return NULL;
}

struct ufs_buffer_head * ubh_bread_uspi (struct ufs_sb_private_info * uspi,
	struct super_block *sb, unsigned fragment, unsigned size)
{
	unsigned i, j, count;
	if (size & ~uspi->s_fmask)
		return NULL;
	count = size >> uspi->s_fshift;
	if (count <= 0 || count > UFS_MAXFRAG)
		return NULL;
	USPI_UBH->fragment = fragment;
	USPI_UBH->count = count;
	for (i = 0; i < count; i++)
		if (!(USPI_UBH->bh[i] = sb_bread(sb, fragment + i)))
			goto failed;
	for (; i < UFS_MAXFRAG; i++)
		USPI_UBH->bh[i] = NULL;
	return USPI_UBH;
failed:
	for (j = 0; j < i; j++)
		brelse (USPI_UBH->bh[j]);
	return NULL;
}

void ubh_brelse (struct ufs_buffer_head * ubh)
{
	unsigned i;
	if (!ubh)
		return;
	for (i = 0; i < ubh->count; i++)
		brelse (ubh->bh[i]);
	kfree (ubh);
}

void ubh_brelse_uspi (struct ufs_sb_private_info * uspi)
{
	unsigned i;
	if (!USPI_UBH)
		return;
	for ( i = 0; i < USPI_UBH->count; i++ ) {
		brelse (USPI_UBH->bh[i]);
		USPI_UBH->bh[i] = NULL;
	}
}

void ubh_mark_buffer_dirty (struct ufs_buffer_head * ubh)
{
	unsigned i;
	if (!ubh)
		return;
	for ( i = 0; i < ubh->count; i++ )
		mark_buffer_dirty (ubh->bh[i]);
}

void ubh_mark_buffer_uptodate (struct ufs_buffer_head * ubh, int flag)
{
	unsigned i;
	if (!ubh)
		return;
	for ( i = 0; i < ubh->count; i++ )
		mark_buffer_uptodate (ubh->bh[i], flag);
}

void ubh_ll_rw_block (int rw, unsigned nr, struct ufs_buffer_head * ubh[])
{
	unsigned i;
	if (!ubh)
		return;
	for ( i = 0; i < nr; i++ )
		ll_rw_block (rw, ubh[i]->count, ubh[i]->bh);
}

void ubh_wait_on_buffer (struct ufs_buffer_head * ubh)
{
	unsigned i;
	if (!ubh)
		return;
	for ( i = 0; i < ubh->count; i++ )
		wait_on_buffer (ubh->bh[i]);
}

unsigned ubh_max_bcount (struct ufs_buffer_head * ubh)
{
	unsigned i;
	unsigned max = 0;
	if (!ubh)
		return 0;
	for ( i = 0; i < ubh->count; i++ ) 
		if ( atomic_read(&ubh->bh[i]->b_count) > max )
			max = atomic_read(&ubh->bh[i]->b_count);
	return max;
}

void ubh_bforget (struct ufs_buffer_head * ubh)
{
	unsigned i;
	if (!ubh) 
		return;
	for ( i = 0; i < ubh->count; i++ ) if ( ubh->bh[i] ) 
		bforget (ubh->bh[i]);
}
 
int ubh_buffer_dirty (struct ufs_buffer_head * ubh)
{
	unsigned i;
	unsigned result = 0;
	if (!ubh)
		return 0;
	for ( i = 0; i < ubh->count; i++ )
		result |= buffer_dirty(ubh->bh[i]);
	return result;
}

void _ubh_ubhcpymem_(struct ufs_sb_private_info * uspi, 
	unsigned char * mem, struct ufs_buffer_head * ubh, unsigned size)
{
	unsigned len, bhno;
	if (size > (ubh->count << uspi->s_fshift))
		size = ubh->count << uspi->s_fshift;
	bhno = 0;
	while (size) {
		len = min_t(unsigned int, size, uspi->s_fsize);
		memcpy (mem, ubh->bh[bhno]->b_data, len);
		mem += uspi->s_fsize;
		size -= len;
		bhno++;
	}
}

void _ubh_memcpyubh_(struct ufs_sb_private_info * uspi, 
	struct ufs_buffer_head * ubh, unsigned char * mem, unsigned size)
{
	unsigned len, bhno;
	if (size > (ubh->count << uspi->s_fshift))
		size = ubh->count << uspi->s_fshift;
	bhno = 0;
	while (size) {
		len = min_t(unsigned int, size, uspi->s_fsize);
		memcpy (ubh->bh[bhno]->b_data, mem, len);
		mem += uspi->s_fsize;
		size -= len;
		bhno++;
	}
}
