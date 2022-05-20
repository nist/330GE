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
/* $Id: hysdn_if.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 *
 * Linux driver for HYSDN cards
 * ioctl definitions shared by hynetmgr and driver.
 *
 * Author    Werner Cornelius (werner@titro.de) for Hypercope GmbH
 * Copyright 1999 by Werner Cornelius (werner@titro.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

/****************/
/* error values */
/****************/
#define ERR_NONE             0 /* no error occurred */
#define ERR_ALREADY_BOOT  1000 /* we are already booting */
#define EPOF_BAD_MAGIC    1001 /* bad magic in POF header */
#define ERR_BOARD_DPRAM   1002 /* board DPRAM failed */
#define EPOF_INTERNAL     1003 /* internal POF handler error */
#define EPOF_BAD_IMG_SIZE 1004 /* POF boot image size invalid */
#define ERR_BOOTIMG_FAIL  1005 /* 1. stage boot image did not start */
#define ERR_BOOTSEQ_FAIL  1006 /* 2. stage boot seq handshake timeout */
#define ERR_POF_TIMEOUT   1007 /* timeout waiting for card pof ready */
#define ERR_NOT_BOOTED    1008 /* operation only allowed when booted */
#define ERR_CONF_LONG     1009 /* conf line is to long */ 
#define ERR_INV_CHAN      1010 /* invalid channel number */ 
#define ERR_ASYNC_TIME    1011 /* timeout sending async data */ 



