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
 * CFE device driver tunables for
 * Broadcom BCM47XX 10/100Mbps Ethernet Device Driver

 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id$
 */

#ifndef _et_cfe_h_
#define _et_cfe_h_

#define NTXD	        16
#define NRXD	        16
#define NRXBUFPOST	8   

#define NBUFS		(NRXBUFPOST + 8)

/* common tunables */
#define	RXBUFSZ		LBDATASZ	/* rx packet buffer size */
#define	MAXMULTILIST	32		/* max # multicast addresses */

#endif /* _et_cfe_h_ */
