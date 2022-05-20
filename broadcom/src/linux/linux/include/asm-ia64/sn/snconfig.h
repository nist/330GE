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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000-2001 Silicon Graphics, Inc.
 */
#ifndef _ASM_IA64_SN_SNCONFIG_H
#define _ASM_IA64_SN_SNCONFIG_H

#include <linux/config.h>

#if defined(CONFIG_IA64_SGI_SN1)
#include <asm/sn/sn1/ip27config.h>
#elif defined(CONFIG_IA64_SGI_SN2)
#endif

#endif /* _ASM_IA64_SN_SNCONFIG_H */
