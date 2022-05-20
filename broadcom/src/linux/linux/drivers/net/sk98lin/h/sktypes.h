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
/******************************************************************************
 *
 * Name:	sktypes.h
 * Project:	GEnesis, PCI Gigabit Ethernet Adapter
 * Version:	$Revision: 1.1.1.1 $
 * Date:	$Date: 2007/01/25 12:51:56 $
 * Purpose:	Define data types for Linux
 *
 ******************************************************************************/

/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/
 
 /*****************************************************************************
 *
 * History:
 *
 *	$Log: sktypes.h,v $
 *	Revision 1.1.1.1  2007/01/25 12:51:56  jiahao_jhou
 *	
 *	
 *	Revision 1.1.1.1  2003/02/03 22:37:48  mhuang
 *	LINUX_2_4 branch snapshot from linux-mips.org CVS
 *	
 *	Revision 1.2  1999/11/22 14:01:58  cgoos
 *	Changed license header to GPL.
 *	Now using Linux' fixed size types instead of standard types.
 *	
 *	Revision 1.1  1999/02/16 07:41:40  cgoos
 *	First version.
 *	
 *	
 *
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *
 * In this file, all data types that are needed by the common modules
 * are mapped to Linux data types.
 * 
 *
 * Include File Hierarchy:
 *
 *
 ******************************************************************************/

#ifndef __INC_SKTYPES_H
#define __INC_SKTYPES_H


/* defines *******************************************************************/

/*
 * Data types with a specific size. 'I' = signed, 'U' = unsigned.
 */
#define SK_I8	s8
#define SK_U8	u8
#define SK_I16	s16
#define SK_U16	u16
#define SK_I32	s32
#define SK_U32	u32
#define SK_I64	s64
#define SK_U64	u64

#define SK_UPTR	ulong		/* casting pointer <-> integral */

/*
* Boolean type.
*/
#define SK_BOOL		SK_U8
#define SK_FALSE	0
#define SK_TRUE		(!SK_FALSE)

/* typedefs *******************************************************************/

/* function prototypes ********************************************************/

#endif	/* __INC_SKTYPES_H */
