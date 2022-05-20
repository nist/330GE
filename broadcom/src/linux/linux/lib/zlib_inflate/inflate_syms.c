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
 * linux/lib/zlib_inflate/inflate_syms.c
 *
 * Exported symbols for the inflate functionality.
 *
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/zlib.h>

EXPORT_SYMBOL(zlib_inflate_workspacesize);
EXPORT_SYMBOL(zlib_inflate);
EXPORT_SYMBOL(zlib_inflateInit_);
EXPORT_SYMBOL(zlib_inflateInit2_);
EXPORT_SYMBOL(zlib_inflateEnd);
EXPORT_SYMBOL(zlib_inflateSync);
EXPORT_SYMBOL(zlib_inflateReset);
EXPORT_SYMBOL(zlib_inflateSyncPoint);
EXPORT_SYMBOL(zlib_inflateIncomp); 
MODULE_LICENSE("GPL");
