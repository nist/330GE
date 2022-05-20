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
 * Name:	version.h
 * Project:	GEnesis, PCI Gigabit Ethernet Adapter
 * Version:	$Revision: 1.1.1.1 $
 * Date:	$Date: 2007/01/25 12:51:56 $
 * Purpose:	SK specific Error log support
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

/******************************************************************************
 *
 * History:
 *	$Log: skversion.h,v $
 *	Revision 1.1.1.1  2007/01/25 12:51:56  jiahao_jhou
 *	
 *	
 *	Revision 1.1.1.1  2003/02/03 22:37:48  mhuang
 *	LINUX_2_4 branch snapshot from linux-mips.org CVS
 *	
 *	Revision 1.1  2001/03/06 09:25:00  mlindner
 *	first version
 *	
 *	
 *
 ******************************************************************************/
 
 
static const char SysKonnectFileId[] = "@(#)" __FILE__ " (C) SysKonnect GmbH.";
static const char SysKonnectBuildNumber[] =
	"@(#)SK-BUILD: 4.06 PL: 01"; 

#define BOOT_STRING	"sk98lin: Network Device Driver v4.06\n" \
			"Copyright (C) 2000-2001 SysKonnect GmbH."

#define VER_STRING	"4.06"


