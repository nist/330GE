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
 * BK Id: %F% %I% %G% %U% %#%
 */

/* This is the single file included by all MPC8xx build options.
 * Since there are many different boards and no standard configuration,
 * we have a unique include file for each.  Rather than change every
 * file that has to include MPC8xx configuration, they all include
 * this one and the configuration switching is done here.
 */
#ifdef __KERNEL__
#ifndef __CONFIG_8xx_DEFS
#define __CONFIG_8xx_DEFS

#include <linux/config.h>

#ifdef CONFIG_8xx

#ifdef CONFIG_MBX
#include <platforms/mbx.h>
#endif

#ifdef CONFIG_FADS
#include <platforms/fads.h>
#endif

#ifdef CONFIG_RPXLITE
#include <platforms/rpxlite.h>
#endif

#ifdef CONFIG_BSEIP
#include <platforms/bseip.h>
#endif

#ifdef CONFIG_RPXCLASSIC
#include <platforms/rpxclassic.h>
#endif

#if defined(CONFIG_TQM8xxL)
#include <platforms/tqm8xx.h>
#endif

#if defined(CONFIG_SPD823TS)
#include <platforms/spd8xx.h>
#endif

#if defined(CONFIG_IVMS8) || defined(CONFIG_IVML24)
#include <platforms/ivms8.h>
#endif


/* Currently, all 8xx boards that support a processor to PCI/ISA bridge
 * use the same memory map.
 */
#if !defined(_IO_BASE)      /* defined in board specific header */
#define _IO_BASE        0
#endif
#define _ISA_MEM_BASE   0
#define PCI_DRAM_OFFSET 0

#ifndef __ASSEMBLY__
extern unsigned long isa_io_base;
extern unsigned long isa_mem_base;
extern unsigned long pci_dram_offset;

/* The "residual" data board information structure the boot loader
 * hands to us.
 */
extern unsigned char __res[];

struct pt_regs;
extern int request_8xxirq(unsigned int irq,
		       void (*handler)(int, void *, struct pt_regs *),
		       unsigned long flags, 
		       const char *device,
		       void *dev_id);
#endif /* !__ASSEMBLY__ */
#endif /* CONFIG_8xx */
#endif /* __CONFIG_8xx_DEFS */
#endif /* __KERNEL__ */
