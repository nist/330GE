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
 *
 *
 */

#include <linux/types.h>

#define ReservedMemVirtualAddr  0x50000000

unsigned long get_mem_avail(void);

ulong* get_reserved_buffer(void);
ulong* get_reserved_buffer_virtual(void);
ulong get_reserved_buffer_size(void);

void  reserve_buffer(const char* cl, ulong base_mem);


