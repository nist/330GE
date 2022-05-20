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
 * $Id: rpxlite.c,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * Handle mapping of the flash on the RPX Lite and CLLF boards
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>


#define WINDOW_ADDR 0xfe000000
#define WINDOW_SIZE 0x800000

static struct mtd_info *mymtd;

__u8 rpxlite_read8(struct map_info *map, unsigned long ofs)
{
	return __raw_readb(map->map_priv_1 + ofs);
}

__u16 rpxlite_read16(struct map_info *map, unsigned long ofs)
{
	return __raw_readw(map->map_priv_1 + ofs);
}

__u32 rpxlite_read32(struct map_info *map, unsigned long ofs)
{
	return __raw_readl(map->map_priv_1 + ofs);
}

void rpxlite_copy_from(struct map_info *map, void *to, unsigned long from, ssize_t len)
{
	memcpy_fromio(to, (void *)(map->map_priv_1 + from), len);
}

void rpxlite_write8(struct map_info *map, __u8 d, unsigned long adr)
{
	__raw_writeb(d, map->map_priv_1 + adr);
	mb();
}

void rpxlite_write16(struct map_info *map, __u16 d, unsigned long adr)
{
	__raw_writew(d, map->map_priv_1 + adr);
	mb();
}

void rpxlite_write32(struct map_info *map, __u32 d, unsigned long adr)
{
	__raw_writel(d, map->map_priv_1 + adr);
	mb();
}

void rpxlite_copy_to(struct map_info *map, unsigned long to, const void *from, ssize_t len)
{
	memcpy_toio((void *)(map->map_priv_1 + to), from, len);
}

struct map_info rpxlite_map = {
	name: "RPX",
	size: WINDOW_SIZE,
	buswidth: 4,
	read8: rpxlite_read8,
	read16: rpxlite_read16,
	read32: rpxlite_read32,
	copy_from: rpxlite_copy_from,
	write8: rpxlite_write8,
	write16: rpxlite_write16,
	write32: rpxlite_write32,
	copy_to: rpxlite_copy_to
};

int __init init_rpxlite(void)
{
	printk(KERN_NOTICE "RPX Lite or CLLF flash device: %x at %x\n", WINDOW_SIZE*4, WINDOW_ADDR);
	rpxlite_map.map_priv_1 = (unsigned long)ioremap(WINDOW_ADDR, WINDOW_SIZE * 4);

	if (!rpxlite_map.map_priv_1) {
		printk("Failed to ioremap\n");
		return -EIO;
	}
	mymtd = do_map_probe("cfi_probe", &rpxlite_map);
	if (mymtd) {
		mymtd->module = THIS_MODULE;
		add_mtd_device(mymtd);
		return 0;
	}

	iounmap((void *)rpxlite_map.map_priv_1);
	return -ENXIO;
}

static void __exit cleanup_rpxlite(void)
{
	if (mymtd) {
		del_mtd_device(mymtd);
		map_destroy(mymtd);
	}
	if (rpxlite_map.map_priv_1) {
		iounmap((void *)rpxlite_map.map_priv_1);
		rpxlite_map.map_priv_1 = 0;
	}
}

module_init(init_rpxlite);
module_exit(cleanup_rpxlite);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnold Christensen <AKC@pel.dk>");
MODULE_DESCRIPTION("MTD map driver for RPX Lite and CLLF boards");
