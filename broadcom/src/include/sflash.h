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
 * Broadcom SiliconBackplane chipcommon serial flash interface
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: sflash.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _sflash_h_
#define _sflash_h_

#include <typedefs.h>
#include <sbchipc.h>

struct sflash {
	uint blocksize;		/* Block size */
	uint numblocks;		/* Number of blocks */
	uint32 type;		/* Type */
	uint size;		/* Total size in bytes */
};

/* Utility functions */
extern int sflash_poll(sb_t *sbh, chipcregs_t *cc, uint offset);
extern int sflash_read(sb_t *sbh, chipcregs_t *cc,
                       uint offset, uint len, uchar *buf);
extern int sflash_write(sb_t *sbh, chipcregs_t *cc,
                        uint offset, uint len, const uchar *buf);
extern int sflash_erase(sb_t *sbh, chipcregs_t *cc, uint offset);
extern int sflash_commit(sb_t *sbh, chipcregs_t *cc,
                         uint offset, uint len, const uchar *buf);
extern struct sflash *sflash_init(sb_t *sbh, chipcregs_t *cc);

#endif /* _sflash_h_ */
