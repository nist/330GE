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
/* Exported symbols for NuBus services

   (c) 1999 David Huggins-Daines <dhd@debian.org> */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/nubus.h>

#ifdef CONFIG_PROC_FS
EXPORT_SYMBOL(nubus_proc_attach_device);
EXPORT_SYMBOL(nubus_proc_detach_device);
#endif

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(nubus_find_device);
EXPORT_SYMBOL(nubus_find_type);
EXPORT_SYMBOL(nubus_find_slot);
EXPORT_SYMBOL(nubus_get_root_dir);
EXPORT_SYMBOL(nubus_get_board_dir);
EXPORT_SYMBOL(nubus_get_func_dir);
EXPORT_SYMBOL(nubus_readdir);
EXPORT_SYMBOL(nubus_find_rsrc);
EXPORT_SYMBOL(nubus_rewinddir);
EXPORT_SYMBOL(nubus_get_subdir);
EXPORT_SYMBOL(nubus_get_rsrc_mem);
EXPORT_SYMBOL(nubus_get_rsrc_str);

