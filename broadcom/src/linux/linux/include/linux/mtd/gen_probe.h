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
 * (C) 2001, 2001 Red Hat, Inc.
 * GPL'd
 * $Id: gen_probe.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 */

#ifndef __LINUX_MTD_GEN_PROBE_H__
#define __LINUX_MTD_GEN_PROBE_H__

#include <linux/mtd/flashchip.h>
#include <linux/mtd/map.h> 
#include <linux/mtd/cfi.h>

struct chip_probe {
	char *name;
	int (*probe_chip)(struct map_info *map, __u32 base,
			  struct flchip *chips, struct cfi_private *cfi);

};

struct mtd_info *mtd_do_chip_probe(struct map_info *map, struct chip_probe *cp);

#endif /* __LINUX_MTD_GEN_PROBE_H__ */
