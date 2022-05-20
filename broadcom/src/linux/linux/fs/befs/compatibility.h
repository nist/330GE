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
 * linux/fs/befs/compatiblity.h
 *
 * Copyright (C) 2001 Will Dyson <will_dyson@pobox.com>
 *   AKA <will@cs.earlham.edu>
 *
 * This file trys to take care of differences between
 * kernel versions
 */

#include <linux/version.h>

/* New interfaces in 2.4.10 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,10)

#define min_t(type,x,y) \
({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })

#define max_t(type,x,y) \
({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

#define vsnprintf(buf, n, fmt, args) vsprintf(buf, fmt, args)

#define MODULE_LICENSE(x)

#endif				/* LINUX_VERSION_CODE */
