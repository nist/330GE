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
 *  linux/drivers/ide/falconide.c -- Atari Falcon IDE Driver
 *
 *     Created 12 Jul 1997 by Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/ide.h>
#include <linux/init.h>

#include <asm/setup.h>
#include <asm/atarihw.h>
#include <asm/atariints.h>
#include <asm/atari_stdma.h>


    /*
     *  Base of the IDE interface
     */

#define ATA_HD_BASE	0xfff00000

    /*
     *  Offsets from the above base
     */

#define ATA_HD_DATA	0x00
#define ATA_HD_ERROR	0x05		/* see err-bits */
#define ATA_HD_NSECTOR	0x09		/* nr of sectors to read/write */
#define ATA_HD_SECTOR	0x0d		/* starting sector */
#define ATA_HD_LCYL	0x11		/* starting cylinder */
#define ATA_HD_HCYL	0x15		/* high byte of starting cyl */
#define ATA_HD_SELECT	0x19		/* 101dhhhh , d=drive, hhhh=head */
#define ATA_HD_STATUS	0x1d		/* see status-bits */
#define ATA_HD_CONTROL	0x39

static int falconide_offsets[IDE_NR_PORTS] __initdata = {
    ATA_HD_DATA, ATA_HD_ERROR, ATA_HD_NSECTOR, ATA_HD_SECTOR, ATA_HD_LCYL,
    ATA_HD_HCYL, ATA_HD_SELECT, ATA_HD_STATUS, ATA_HD_CONTROL, -1
};


    /*
     *  Probe for a Falcon IDE interface
     */

void __init falconide_init(void)
{
    if (MACH_IS_ATARI && ATARIHW_PRESENT(IDE)) {
	hw_regs_t hw;
	int index;

	ide_setup_ports(&hw, (ide_ioreg_t)ATA_HD_BASE, falconide_offsets,
			0, 0, NULL, IRQ_MFP_IDE);
	index = ide_register_hw(&hw, NULL);

	if (index != -1)
	    printk("ide%d: Falcon IDE interface\n", index);
    }
}
