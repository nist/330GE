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
 * Stubs for NVRAM functions for platforms without flash
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvramstubs.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>
#include <bcmutils.h>
#include <bcmnvram.h>
#undef strcmp
#define strcmp(s1,s2)	0	/* always match */

int
nvram_init(void *sbh)
{
	return 0;
}

void
nvram_exit(void *sbh)
{
}

char *
nvram_get(const char *name)
{
	return (char *) 0;
}

int
nvram_set(const char *name, const char *value)
{
	return 0;
}

int
nvram_unset(const char *name)
{
	return 0;
}

int
nvram_commit(void)
{
	return 0;
}

int
nvram_getall(char *buf, int count)
{
	/* add null string as terminator */
	if (count < 1)
		return BCME_BUFTOOSHORT;
	*buf = '\0';
	return 0;
}
