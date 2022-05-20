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
 * NDIS Error codes
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id: ndiserrmap.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */
#ifndef NDIS
#include <bcm_ndis.h>
#endif
#include <typedefs.h>
#include <osl.h>
#include <epivers.h>
#include <bcmutils.h>

#include <ndiserrmap.h>


static NDIS_STATUS ndisbcmerrormap[] =  \
{	NDIS_STATUS_SUCCESS, 			/* 0 */
	NDIS_STATUS_FAILURE,			/* BCME_ERROR */
	NDIS_STATUS_INVALID_DATA,		/* BCME_BADARG */
	NDIS_STATUS_INVALID_DATA,		/* BCME_BADOPTION */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOTUP */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOTDOWN */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOTAP */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOTSTA */
	NDIS_STATUS_INVALID_DATA,		/* BCME_BADKEYIDX */
	NDIS_STATUS_INVALID_DATA,		/* BCME_RADIOOFF */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOTBANDLOCKED */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_NOCLK */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_BADRATESET */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_BADBAND */
	NDIS_STATUS_INVALID_LENGTH,		/* BCME_BUFTOOSHORT */
	NDIS_STATUS_INVALID_LENGTH,		/* BCME_BUFTOOLONG */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_BUSY */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_NOTASSOCIATED */
	NDIS_STATUS_INVALID_LENGTH, 		/* BCME_BADSSIDLEN */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_OUTOFRANGECHAN */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_BADCHAN */
	NDIS_STATUS_INVALID_DATA, 		/* BCME_BADADDR */
	NDIS_STATUS_RESOURCES, 			/* BCME_NORESOURCE */
	NDIS_STATUS_NOT_SUPPORTED,		/* BCME_UNSUPPORTED */
	NDIS_STATUS_INVALID_LENGTH,		/* BCME_BADLENGTH */
	NDIS_STATUS_ADAPTER_NOT_READY,		/* BCME_NOTREADY */
	NDIS_STATUS_FAILURE,			/* BCME_NOTPERMITTED */
	NDIS_STATUS_RESOURCES, 			/* BCME_NOMEM */
	NDIS_STATUS_INVALID_DATA,		/* BCME_ASSOCIATED */
	NDIS_STATUS_INVALID_DATA,		/* BCME_RANGE */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOTFOUND */
	NDIS_STATUS_INVALID_DATA,		/* BCME_WME_NOT_ENABLED */
	NDIS_STATUS_INVALID_DATA,		/* BCME_TSPEC_NOTFOUND */
	NDIS_STATUS_INVALID_DATA,		/* BCME_ACM_NOTSUPPORTED */
	NDIS_STATUS_INVALID_DATA,		/* BCME_NOT_WME_ASSOCIATION */
	NDIS_STATUS_FAILURE,			/* BCME_SDIO_ERROR */
	NDIS_STATUS_FAILURE			/* BCME_DONGLE_DOWN */
	};

int
ndisstatus2bcmerror(NDIS_STATUS status)
{
	int array_size = ARRAYSIZE(ndisbcmerrormap);
	int i = 0;


	ASSERT(ABS(BCME_LAST) == (array_size - 1));

	while (i < array_size) {
		if (ndisbcmerrormap[i] == status)
			return -i;
		i++;
	}
	if (status == NDIS_STATUS_INVALID_OID)
		return BCME_UNSUPPORTED;

	return BCME_ERROR;
}

NDIS_STATUS
bcmerror2ndisstatus(int bcmerror)
{
	int abs_bcmerror;
	int array_size = ARRAYSIZE(ndisbcmerrormap);

	abs_bcmerror = ABS(bcmerror);

	if (bcmerror > 0)
		abs_bcmerror = 0;

	else if (abs_bcmerror >= array_size)
		abs_bcmerror = BCME_ERROR;

	return ndisbcmerrormap[abs_bcmerror];
}
