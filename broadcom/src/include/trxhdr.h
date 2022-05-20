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
 * TRX image file header format.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: trxhdr.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>

#define TRX_MAGIC	0x30524448	/* "HDR0" */
#define TRX_VERSION	1		/* Version 1 */
#define TRX_MAX_LEN	0x7D0000	/* Max length */	// 2007.02.12 Jiahao
#define TRX_NO_HEADER	1		/* Do not write TRX header */
#define TRX_GZ_FILES	0x2     /* Contains up to TRX_MAX_OFFSET individual gzip files */
#define TRX_MAX_OFFSET	3		/* Max number of individual files */

struct trx_header {
	uint32 magic;		/* "HDR0" */
	uint32 len;		/* Length of file including header */
	uint32 crc32;		/* 32-bit CRC from flag_version to end of file */
	uint32 flag_version;	/* 0:15 flags, 16:31 version */
	uint32 offsets[TRX_MAX_OFFSET];	/* Offsets of partitions from start of header */
};

/* Compatibility */
typedef struct trx_header TRXHDR, *PTRXHDR;
