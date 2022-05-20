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
/* -*- linux-c -*- */
/*
 * Copyright (C) 2001 By Joachim Martillo, Telford Tools, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 */

/****************************************************/
/****************************************************/
/*	    Begin source file "crc32dcl.h"	    */
/****************************************************/
/****************************************************/

#if !defined(_CRC32_HP_)
#define _CRC32_HP_

/****************************************************/
/*		    header files		    */
/****************************************************/

#include "crc32.h"

/****************************************************/
/*	    global procedure prototypes		    */
/****************************************************/

extern void	fn_init_crc_table(void);
extern unsigned int	fn_calc_memory_chunk_crc32(void *p, unsigned int n_bytes, unsigned int crc);
extern unsigned int	fn_calc_memory_crc32(void *p, unsigned int n_bytes);
extern unsigned int	fn_check_memory_crc32(void *p, unsigned int n_bytes, unsigned int crc);

extern unsigned int gg_a_crc_table[];


#endif

/****************************************************/
/****************************************************/
/*	    End source file "crc32dcl.h"		    */
/****************************************************/
/****************************************************/
