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


#include <net/checksum.h>

unsigned int csum_partial_copy( const char *src, char *dst, int len, int sum)
{
	int src_err=0, dst_err=0;

	sum = csum_partial_copy_generic ( src, dst, len, sum, &src_err, &dst_err);

	if (src_err || dst_err)
		printk("old csum_partial_copy_fromuser(), tell mingo to convert me.\n");

	return sum;
}


