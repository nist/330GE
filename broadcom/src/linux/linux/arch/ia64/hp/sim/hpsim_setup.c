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
 * Platform dependent support for HP simulator.
 *
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 Vijay Chander <vijay@engr.sgi.com>
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/console.h>

#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/pal.h>
#include <asm/machvec.h>
#include <asm/pgtable.h>
#include <asm/sal.h>

#include "hpsim_ssc.h"

extern struct console hpsim_cons;

/*
 * Simulator system call.
 */
asm (".text\n"
     ".align 32\n"
     ".global ia64_ssc\n"
     ".proc ia64_ssc\n"
     "ia64_ssc:\n"
     "mov r15=r36\n"
     "break 0x80001\n"
     "br.ret.sptk.many rp\n"
     ".endp\n");

void
ia64_ssc_connect_irq (long intr, long irq)
{
	ia64_ssc(intr, irq, 0, 0, SSC_CONNECT_INTERRUPT);
}

void
ia64_ctl_trace (long on)
{
	ia64_ssc(on, 0, 0, 0, SSC_CTL_TRACE);
}

void __init
hpsim_setup (char **cmdline_p)
{
	ROOT_DEV = to_kdev_t(0x0801);		/* default to first SCSI drive */

	register_console (&hpsim_cons);
}
