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
/* $Id: isdn_x25iface.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * header for Linux ISDN subsystem, x.25 related functions
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef _LINUX_ISDN_X25IFACE_H
#define _LINUX_ISDN_X25IFACE_H

#define ISDN_X25IFACE_MAGIC 0x1e75a2b9
/* #define DEBUG_ISDN_X25 if you want isdn_x25 debugging messages */
#ifdef DEBUG_ISDN_X25
#   define IX25DEBUG(fmt,args...) printk(KERN_DEBUG fmt , ## args)
#else
#   define IX25DEBUG(fmt,args...)
#endif

#include <linux/skbuff.h>
#include <linux/wanrouter.h>
#include <linux/isdn.h>
#include <linux/concap.h>

extern struct concap_proto_ops * isdn_x25iface_concap_proto_ops_pt;
extern struct concap_proto     * isdn_x25iface_proto_new(void);



#endif








