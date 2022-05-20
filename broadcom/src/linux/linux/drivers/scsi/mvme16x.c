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
 * Detection routine for the NCR53c710 based MVME16x SCSI Controllers for Linux.
 *
 * Based on work by Alan Hourihane
 */
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/blk.h>
#include <linux/sched.h>
#include <linux/version.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mvme16xhw.h>
#include <asm/irq.h>

#include "scsi.h"
#include "hosts.h"
#include "53c7xx.h"
#include "mvme16x.h"

#include<linux/stat.h>

extern int ncr53c7xx_init(Scsi_Host_Template *tpnt, int board, int chip,
			  u32 base, int io_port, int irq, int dma,
			  long long options, int clock);

int mvme16x_scsi_detect(Scsi_Host_Template *tpnt)
{
    static unsigned char called = 0;
    int clock;
    long long options;

    if (!MACH_IS_MVME16x)
		return 0;
    if (mvme16x_config & MVME16x_CONFIG_NO_SCSICHIP) {
	printk ("SCSI detection disabled, SCSI chip not present\n");
	return 0;
    }
    if (called)
	return 0;

    tpnt->proc_name = "MVME16x";

    options = OPTION_MEMORY_MAPPED|OPTION_DEBUG_TEST1|OPTION_INTFLY|OPTION_SYNCHRONOUS|OPTION_ALWAYS_SYNCHRONOUS|OPTION_DISCONNECT;

    clock = 66000000;	/* 66MHz SCSI Clock */

    ncr53c7xx_init(tpnt, 0, 710, (u32)0xfff47000,
			0, MVME16x_IRQ_SCSI, DMA_NONE,
			options, clock);
    called = 1;
    return 1;
}

static Scsi_Host_Template driver_template = MVME16x_SCSI;
#include "scsi_module.c"
