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
 * 
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 * 
 * Copyright (C) 2001-2002 Silicon Graphics, Inc. All rights reserved.
 *
 */

#include <asm/pgalloc.h>

/**
 * sn_flush_all_caches - flush a range of address from all caches (incl. L4)
 * @flush_addr: identity mapped region 7 address to start flushing
 * @bytes: number of bytes to flush
 *
 * Flush a range of addresses from all caches including L4. 
 * All addresses fully or partially contained within 
 * @flush_addr to @flush_addr + @bytes are flushed
 * from the all caches.
 */
void
sn_flush_all_caches(long flush_addr, long bytes)
{
	flush_icache_range(flush_addr, flush_addr+bytes);
}


