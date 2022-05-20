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
 * Common code to handle map devices which are simple RAM
 * (C) 2000 Red Hat. GPL'd.
 * $Id: map_ram.c,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include <linux/mtd/map.h>


static int mapram_read (struct mtd_info *, loff_t, size_t, size_t *, u_char *);
static int mapram_write (struct mtd_info *, loff_t, size_t, size_t *, const u_char *);
static int mapram_erase (struct mtd_info *, struct erase_info *);
static void mapram_nop (struct mtd_info *);
static struct mtd_info *map_ram_probe(struct map_info *map);


static struct mtd_chip_driver mapram_chipdrv = {
	probe: map_ram_probe,
	name: "map_ram",
	module: THIS_MODULE
};

static struct mtd_info *map_ram_probe(struct map_info *map)
{
	struct mtd_info *mtd;

	/* Check the first byte is RAM */
	/* OK. It seems to be RAM. */

	mtd = kmalloc(sizeof(*mtd), GFP_KERNEL);
	if (!mtd)
		return NULL;

	memset(mtd, 0, sizeof(*mtd));

	map->fldrv = &mapram_chipdrv;
	mtd->priv = map;
	mtd->name = map->name;
	mtd->type = MTD_RAM;
	mtd->size = map->size;
	mtd->erase = mapram_erase;
	mtd->read = mapram_read;
	mtd->write = mapram_write;
	mtd->sync = mapram_nop;
	mtd->flags = MTD_CAP_RAM | MTD_VOLATILE;

	mtd->erasesize = PAGE_SIZE;
 	while(mtd->size & (mtd->erasesize - 1))
		mtd->erasesize >>= 1;

	MOD_INC_USE_COUNT;
	return mtd;
}


static int mapram_read (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct map_info *map = (struct map_info *)mtd->priv;

	map->copy_from(map, buf, from, len);
	*retlen = len;
	return 0;
}

static int mapram_write (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	struct map_info *map = (struct map_info *)mtd->priv;

	map->copy_to(map, to, buf, len);
	*retlen = len;
	return 0;
}

static int mapram_erase (struct mtd_info *mtd, struct erase_info *instr)
{
	/* Yeah, it's inefficient. Who cares? It's faster than a _real_
	   flash erase. */
	struct map_info *map = (struct map_info *)mtd->priv;
	unsigned long i;

	for (i=0; i<instr->len; i++)
		map->write8(map, 0xFF, instr->addr + i);

	if (instr->callback)
		instr->callback(instr);

	return 0;
}

static void mapram_nop(struct mtd_info *mtd)
{
	/* Nothing to see here */
}

int __init map_ram_init(void)
{
	register_mtd_chip_driver(&mapram_chipdrv);
	return 0;
}

static void __exit map_ram_exit(void)
{
	unregister_mtd_chip_driver(&mapram_chipdrv);
}

module_init(map_ram_init);
module_exit(map_ram_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("MTD chip driver for RAM chips");