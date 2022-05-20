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
#define __MACHVEC_HDR(n)		<asm/machvec_##n##.h>
#define __MACHVEC_EXPAND(n)		__MACHVEC_HDR(n)
#define MACHVEC_PLATFORM_HEADER		__MACHVEC_EXPAND(MACHVEC_PLATFORM_NAME)

#include <asm/machvec.h>

extern ia64_mv_send_ipi_t ia64_send_ipi;
extern ia64_mv_global_tlb_purge_t ia64_global_tlb_purge;
extern ia64_mv_irq_desc __ia64_irq_desc;
extern ia64_mv_irq_to_vector __ia64_irq_to_vector;
extern ia64_mv_local_vector_to_irq __ia64_local_vector_to_irq;

extern ia64_mv_inb_t __ia64_inb;
extern ia64_mv_inw_t __ia64_inw;
extern ia64_mv_inl_t __ia64_inl;
extern ia64_mv_outb_t __ia64_outb;
extern ia64_mv_outw_t __ia64_outw;
extern ia64_mv_outl_t __ia64_outl;

#define MACHVEC_HELPER(name)									\
 struct ia64_machine_vector machvec_##name __attribute__ ((unused, __section__ (".machvec")))	\
	= MACHVEC_INIT(name);

#define MACHVEC_DEFINE(name)	MACHVEC_HELPER(name)

MACHVEC_DEFINE(MACHVEC_PLATFORM_NAME)
