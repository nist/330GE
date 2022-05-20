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
#ifndef _ASM_PPC_ZORRO_H
#define _ASM_PPC_ZORRO_H

#include <asm/io.h>

#define z_readb in_8
#define z_readw in_be16
#define z_readl in_be32

#define z_writeb(val, port) out_8((port), (val))
#define z_writew(val, port) out_be16((port), (val))
#define z_writel(val, port) out_be32((port), (val))

#define z_memset_io(a,b,c)	memset((void *)(a),(b),(c))
#define z_memcpy_fromio(a,b,c)	memcpy((a),(void *)(b),(c))
#define z_memcpy_toio(a,b,c)	memcpy((void *)(a),(b),(c))

extern void *__ioremap(unsigned long address, unsigned long size,
		       unsigned long flags);

extern void *ioremap(unsigned long address, unsigned long size);
extern void iounmap(void *addr);

extern void *__ioremap(unsigned long address, unsigned long size,
                       unsigned long flags);

#define z_ioremap ioremap
#define z_iounmap iounmap

#endif /* _ASM_ZORRO_H */
