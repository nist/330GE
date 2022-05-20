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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Miscellaneous ARCS PROM routines.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1999 Ralf Baechle (ralf@gnu.org)
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <asm/bcache.h>

#include <asm/arc/types.h>
#include <asm/sgialib.h>
#include <asm/bootinfo.h>
#include <asm/system.h>

extern void *sgiwd93_host;
extern void reset_wd33c93(void *instance);

VOID
ArcHalt(VOID)
{
	bc_disable();
	cli();
#if CONFIG_SCSI_SGIWD93
	reset_wd33c93(sgiwd93_host);
#endif
	ARC_CALL0(halt);
never:	goto never;
}

VOID
ArcPowerDown(VOID)
{
	bc_disable();
	cli();
#if CONFIG_SCSI_SGIWD93
	reset_wd33c93(sgiwd93_host);
#endif
	ARC_CALL0(pdown);
never:	goto never;
}

VOID
ArcRestart(VOID)
{
	bc_disable();
	cli();
#if CONFIG_SCSI_SGIWD93
	reset_wd33c93(sgiwd93_host);
#endif
	ARC_CALL0(restart);
never:	goto never;
}

VOID
ArcReboot(VOID)
{
	bc_disable();
	cli();
#if CONFIG_SCSI_SGIWD93
	reset_wd33c93(sgiwd93_host);
#endif
	ARC_CALL0(reboot);
never:	goto never;
}

VOID
ArcEnterInteractiveMode(VOID)
{
	bc_disable();
	cli();
#if CONFIG_SCSI_SGIWD93
	reset_wd33c93(sgiwd93_host);
#endif
	ARC_CALL0(imode);
never:	goto never;
}

LONG
ArcSaveConfiguration(VOID)
{
	return ARC_CALL0(cfg_save);
}

struct linux_sysid *
ArcGetSystemId(VOID)
{
	return (struct linux_sysid *) ARC_CALL0(get_sysid);
}

VOID __init
ArcFlushAllCaches(VOID)
{
	ARC_CALL0(cache_flush);
}
