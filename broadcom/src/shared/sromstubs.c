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
 * Broadcom Home Networking Division (HND) srom stubs
 *
 * Should be called bcmsromstubs.c .
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: sromstubs.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <sbutils.h>
#include <bcmsrom.h>

int
srom_var_init(sb_t *sbh, uint bus, void *curmap, osl_t *osh, char **vars, uint *count)
{
	return 0;
}

int
srom_read(sb_t *sbh, uint bus, void *curmap, osl_t *osh, uint byteoff, uint nbytes, uint16 *buf)
{
	return 0;
}

int
srom_write(sb_t *sbh, uint bus, void *curmap, osl_t *osh, uint byteoff, uint nbytes, uint16 *buf)
{
	return 0;
}
