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
/* $Id: loadmmu.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * loadmmu.c:  This code loads up all the mm function pointers once the
 *             machine type has been determined.  It also sets the static
 *             mmu values such as PAGE_NONE, etc.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>

#include <asm/system.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/a.out.h>
#include <asm/mmu_context.h>
#include <asm/oplib.h>

struct ctx_list *ctx_list_pool;
struct ctx_list ctx_free;
struct ctx_list ctx_used;

unsigned int pg_iobits;

extern void ld_mmu_sun4c(void);
extern void ld_mmu_srmmu(void);
extern void ioport_init(void);

void __init load_mmu(void)
{
	switch(sparc_cpu_model) {
	case sun4c:
	case sun4:
		ld_mmu_sun4c();
		break;
	case sun4m:
	case sun4d:
		ld_mmu_srmmu();
		break;
	default:
		prom_printf("load_mmu: %d unsupported\n", (int)sparc_cpu_model);
		prom_halt();
	}
	btfixup();
	ioport_init();
}
