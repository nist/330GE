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
/* pmc - Driver implementation for power management functions
 * of Power Management Controller (PMC) on SPARCstation-Voyager.
 *
 * Copyright (c) 2002 Eric Brower (ebrower@usa.net)
 */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/pm.h>

#include <asm/io.h>
#include <asm/sbus.h>
#include <asm/oplib.h>
#include <asm/uaccess.h>
#include <asm/auxio.h>

/* Debug
 *
 * #define PMC_DEBUG_LED
 * #define PMC_NO_IDLE
 */

#define PMC_MINOR	MISC_DYNAMIC_MINOR
#define PMC_OBPNAME	"SUNW,pmc"
#define PMC_DEVNAME "pmc"

#define PMC_IDLE_REG	0x00
#define PMC_IDLE_ON		0x01

volatile static u8 *regs; 
static int pmc_regsize;

#define pmc_readb(offs)			(sbus_readb(regs+offs))
#define pmc_writeb(val, offs) 	(sbus_writeb(val, regs+offs))

/* 
 * CPU idle callback function
 * See .../arch/sparc/kernel/process.c
 */
void pmc_swift_idle(void)
{
#ifdef PMC_DEBUG_LED
	set_auxio(0x00, AUXIO_LED); 
#endif

	pmc_writeb(pmc_readb(PMC_IDLE_REG) | PMC_IDLE_ON, PMC_IDLE_REG);

#ifdef PMC_DEBUG_LED
	set_auxio(AUXIO_LED, 0x00); 
#endif
} 

static inline void pmc_free(void)
{
	sbus_iounmap((unsigned long)regs, pmc_regsize);
}

static int __init pmc_probe(void)
{
	struct sbus_bus *sbus = NULL;
	struct sbus_dev *sdev = NULL;
	for_each_sbus(sbus) {
		for_each_sbusdev(sdev, sbus) {
			if (!strcmp(sdev->prom_name, PMC_OBPNAME)) {
				goto sbus_done;
			}
		}
	}

sbus_done:
	if (!sdev) {
		return -ENODEV;
	}

	pmc_regsize = sdev->reg_addrs[0].reg_size;
	regs = (u8*) sbus_ioremap(&sdev->resource[0], 0, 
				   pmc_regsize, PMC_OBPNAME);
	if(NULL == regs) {
		printk(KERN_ERR "%s: unable to map registers\n", PMC_DEVNAME);
		return -ENODEV;
	}

#ifndef PMC_NO_IDLE
	/* Assign power management IDLE handler */
	pm_idle = pmc_swift_idle;	
#endif

	printk(KERN_INFO "%s: power management initialized\n", PMC_DEVNAME);
	return 0;
}

/* This driver is not critical to the boot process
 * and is easiest to ioremap when SBus is already
 * initialized, so we install ourselves thusly:
 */
__initcall(pmc_probe);
