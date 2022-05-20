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
 * linux/lib/zlib_deflate/deflate_syms.c
 *
 * Exported symbols for the deflate functionality.
 *
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/zlib.h>

EXPORT_SYMBOL(zlib_deflate_workspacesize);
EXPORT_SYMBOL(zlib_deflate);
EXPORT_SYMBOL(zlib_deflateInit_);
EXPORT_SYMBOL(zlib_deflateInit2_);
EXPORT_SYMBOL(zlib_deflateEnd);
EXPORT_SYMBOL(zlib_deflateReset);
EXPORT_SYMBOL(zlib_deflateCopy);
EXPORT_SYMBOL(zlib_deflateParams);
MODULE_LICENSE("GPL");
