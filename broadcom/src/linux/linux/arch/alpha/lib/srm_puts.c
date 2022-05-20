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
 *	arch/alpha/lib/srm_puts.c
 */

#include <linux/string.h>
#include <asm/console.h>

long
srm_puts(const char *str, long len)
{
	long remaining, written;

	if (!callback_init_done)
		return len;

	for (remaining = len; remaining > 0; remaining -= written)
	{
		written = callback_puts(0, str, remaining);
		written &= 0xffffffff;
		str += written;
	}
	return len;
}
