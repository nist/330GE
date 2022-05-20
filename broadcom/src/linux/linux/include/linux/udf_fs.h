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
 * udf_fs.h
 *
 * PURPOSE
 *  Included by fs/filesystems.c
 *
 * DESCRIPTION
 *  OSTA-UDF(tm) = Optical Storage Technology Association
 *  Universal Disk Format.
 *
 *  This code is based on version 2.00 of the UDF specification,
 *  and revision 3 of the ECMA 167 standard [equivalent to ISO 13346].
 *    http://www.osta.org/ *    http://www.ecma.ch/
 *    http://www.iso.org/
 *
 * CONTACTS
 *	E-mail regarding any portion of the Linux UDF file system should be
 *	directed to the development team mailing list (run by majordomo):
 *		linux_udf@hpesjro.fc.hp.com
 *
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
 *  (C) 1999-2000 Ben Fennema
 *  (C) 1999-2000 Stelias Computing Inc
 *
 * HISTORY
 *
 */
#include <linux/config.h>

#ifndef _UDF_FS_H
#define _UDF_FS_H 1

#define UDF_PREALLOCATE
#define UDF_DEFAULT_PREALLOC_BLOCKS	8

#define UDFFS_DATE			"2002/03/11"
#define UDFFS_VERSION			"0.9.6"

#if !defined(UDFFS_RW)

#if defined(CONFIG_UDF_RW)
#define UDFFS_RW			1
#else /* !defined(CONFIG_UDF_RW) */
#define UDFFS_RW			0
#endif /* defined(CONFIG_UDF_RW) */

#endif /* !defined(UDFFS_RW) */

#define UDFFS_DEBUG

#ifdef UDFFS_DEBUG
#define udf_debug(f, a...) \
	{ \
		printk (KERN_DEBUG "UDF-fs DEBUG %s:%d:%s: ", \
			__FILE__, __LINE__, __FUNCTION__); \
		printk (f, ##a); \
	}
#else
#define udf_debug(f, a...) /**/
#endif

#define udf_info(f, a...) \
		printk (KERN_INFO "UDF-fs INFO " f, ##a);

#endif /* _UDF_FS_H */
