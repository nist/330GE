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
 * add_partition adds a partitions details to the devices partition
 * description.
 */
void add_gd_partition(struct gendisk *hd, int minor, int start, int size);

typedef struct {struct page *v;} Sector;

unsigned char *read_dev_sector(struct block_device *, unsigned long, Sector *);

static inline void put_dev_sector(Sector p)
{
	page_cache_release(p.v);
}

extern int warn_no_part;
