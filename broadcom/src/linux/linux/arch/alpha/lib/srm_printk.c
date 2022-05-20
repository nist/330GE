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
 *	arch/alpha/lib/srm_printk.c
 */

#include <linux/kernel.h>
#include <asm/console.h>

long
srm_printk(const char *fmt, ...)
{
	static char buf[1024];
	va_list args;
	long len, num_lf;
	char *src, *dst;

	va_start(args, fmt);
	len = vsprintf(buf, fmt, args);
	va_end(args);

	/* count number of linefeeds in string: */

	num_lf = 0;
	for (src = buf; *src; ++src) {
		if (*src == '\n') {
			++num_lf;
		}
	}

	if (num_lf) {
		/* expand each linefeed into carriage-return/linefeed: */
		for (dst = src + num_lf; src >= buf; ) {
			if (*src == '\n') {
				*dst-- = '\r';
			}
			*dst-- = *src--;
		}
	}

	srm_puts(buf, num_lf+len);	
        return len;
}
