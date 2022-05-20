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
/* $Id: hubdev.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2001 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_SN1_HUBDEV_H
#define _ASM_IA64_SN_SN1_HUBDEV_H

extern void hubdev_init(void);
extern void hubdev_register(int (*attach_method)(devfs_handle_t));
extern int hubdev_unregister(int (*attach_method)(devfs_handle_t));
extern int hubdev_docallouts(devfs_handle_t hub);

extern caddr_t hubdev_prombase_get(devfs_handle_t hub);
extern cnodeid_t hubdev_cnodeid_get(devfs_handle_t hub);

#endif /* _ASM_IA64_SN_SN1_HUBDEV_H */
