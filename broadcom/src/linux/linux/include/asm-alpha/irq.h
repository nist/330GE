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
#ifndef _ALPHA_IRQ_H
#define _ALPHA_IRQ_H

/*
 *	linux/include/alpha/irq.h
 *
 *	(C) 1994 Linus Torvalds
 */

#include <linux/linkage.h>
#include <linux/config.h>

#if   defined(CONFIG_ALPHA_GENERIC)

/* Here NR_IRQS is not exact, but rather an upper bound.  This is used
   many places throughout the kernel to size static arrays.  That's ok,
   we'll use alpha_mv.nr_irqs when we want the real thing.  */

# define NR_IRQS	2048 /* enuff for WILDFIRE with 8 QBBs */

#elif defined(CONFIG_ALPHA_CABRIOLET) || \
      defined(CONFIG_ALPHA_EB66P)     || \
      defined(CONFIG_ALPHA_EB164)     || \
      defined(CONFIG_ALPHA_PC164)     || \
      defined(CONFIG_ALPHA_LX164)
# define NR_IRQS	35

#elif defined(CONFIG_ALPHA_EB66)      || \
      defined(CONFIG_ALPHA_EB64P)     || \
      defined(CONFIG_ALPHA_MIKASA)
# define NR_IRQS	32

#elif defined(CONFIG_ALPHA_ALCOR)     || \
      defined(CONFIG_ALPHA_XLT)       || \
      defined(CONFIG_ALPHA_MIATA)     || \
      defined(CONFIG_ALPHA_RUFFIAN)   || \
      defined(CONFIG_ALPHA_RX164)     || \
      defined(CONFIG_ALPHA_NORITAKE)
# define NR_IRQS	48

#elif defined(CONFIG_ALPHA_SABLE)     || \
      defined(CONFIG_ALPHA_SX164)
# define NR_IRQS	40

#elif defined(CONFIG_ALPHA_DP264) || \
      defined(CONFIG_ALPHA_SHARK) || \
      defined(CONFIG_ALPHA_EIGER)
# define NR_IRQS	64

#elif defined(CONFIG_ALPHA_TITAN)
#define NR_IRQS		80

#elif defined(CONFIG_ALPHA_RAWHIDE) || \
	defined(CONFIG_ALPHA_TAKARA)
# define NR_IRQS	128

#elif defined(CONFIG_ALPHA_WILDFIRE)
# define NR_IRQS	2048 /* enuff for 8 QBBs */

#else /* everyone else */
# define NR_IRQS	16
#endif

static __inline__ int irq_cannonicalize(int irq)
{
	return ((irq == 2) ? 9 : irq);
}

extern void disable_irq(unsigned int);
extern void disable_irq_nosync(unsigned int);
extern void enable_irq(unsigned int);

struct pt_regs;
extern void (*perf_irq)(unsigned long, struct pt_regs *);


#endif /* _ALPHA_IRQ_H */
