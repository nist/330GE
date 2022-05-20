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
/* $Id: sgi_if.c,v 1.1.1.1 2007/01/25 12:51:48 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2001 Silicon Graphics, Inc. All rights reserved.
 */

#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/sn/sgi.h>
#include <asm/sn/invent.h>
#include <asm/sn/hcl.h>
#include <asm/sn/labelcl.h>
#include <asm/sn/pci/bridge.h>
#include <asm/sn/ioerror_handling.h>
#include <asm/sn/pci/pciio.h>
#include <asm/sn/slotnum.h>

void *
snia_kmem_zalloc(size_t size, int flag)
{
        void *ptr = kmalloc(size, GFP_KERNEL);
        BZERO(ptr, size);
        return ptr;
}

#define xtod(c)         ((c) <= '9' ? '0' - (c) : 'a' - (c) - 10)
long
atoi(register char *p)
{
        register long n;
        register int c, neg = 0;

        if (p == NULL)
                return 0;

        if (!isdigit(c = *p)) {
                while (isspace(c))
                        c = *++p;
                switch (c) {
                case '-':
                        neg++;
                case '+': /* fall-through */
                        c = *++p;
                }
                if (!isdigit(c))
                        return (0);
        }
        if (c == '0' && *(p + 1) == 'x') {
                p += 2;
                c = *p;
                n = xtod(c);
                while ((c = *++p) && isxdigit(c)) {
                        n *= 16; /* two steps to avoid unnecessary overflow */
                        n += xtod(c); /* accum neg to avoid surprises at MAX */
                }
        } else {
                n = '0' - c;
                while ((c = *++p) && isdigit(c)) {
                        n *= 10; /* two steps to avoid unnecessary overflow */
                        n += '0' - c; /* accum neg to avoid surprises at MAX */
                }
        }
        return (neg ? n : -n);
}
