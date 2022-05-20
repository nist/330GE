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
/* $Id: printf.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * printf.c:  Internal prom library printf facility.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

/* This routine is internal to the prom library, no one else should know
 * about or use it!  It's simple and smelly anyway....
 */

#include <linux/kernel.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

static char ppbuf[1024];

extern void prom_puts (char *, int);

void
prom_printf(char *fmt, ...)
{
	va_list args;
	char ch, *bptr, *last;
	int i;

	va_start(args, fmt);
	i = vsprintf(ppbuf, fmt, args);

	bptr = ppbuf;
	last = ppbuf;

	while((ch = *(bptr++)) != 0) {
		if(ch == '\n') {
			if (last < bptr - 1)
				prom_puts (last, bptr - 1 - last);
			prom_putchar('\r');
			last = bptr - 1;
		}
	}
	if (last < bptr - 1)
		prom_puts (last, bptr - 1 - last);
	va_end(args);
	return;
}
