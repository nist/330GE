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
/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  Board device initialization		File: bcm94710_devs.c
    *  
    *  This is the "C" part of the board support package.  The
    *  routines to create and initialize the console, wire up 
    *  device drivers, and do other customization live here.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  XX Copyright 2000,2001
    *  ASUSTek Inc.. All rights reserved.
    *
    *  BROADCOM PROPRIETARY AND CONFIDENTIAL
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the license.
    ********************************************************************* */



#include "sbmips.h"
#include "lib_types.h"
#include "lib_printf.h"
#include "lib_physio.h"
#include "cfe_iocb.h"
#include "cfe_device.h"
#include "cfe_timer.h"
#include "cfe.h"
#include "ui_command.h"
#include "bsp_config.h"
#include "dev_newflash.h"
#include "env_subr.h"
#include "pcivar.h"
#include "pcireg.h"
#include "../../../../../dev/ns16550.h"
#include "net_ebuf.h"
#include "net_ether.h"
#include "net_api.h"

#include <typedefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <sbutils.h>
#include <sbconfig.h>
#include <sbchipc.h>
#include <sbextif.h>
#include <hndpci.h>
#include "bsp_priv.h"
#include <trxhdr.h>
#include <bcmdevs.h>
#include <bcmnvram.h>
#include <hndcpu.h>
#include <hndchipc.h>
#include <epivers.h>

#define MAX_WAIT_TIME 20	/* seconds to wait for boot image */
#define MIN_WAIT_TIME 3 	/* seconds to wait for boot image */

#define RESET_DEBOUNCE_TIME	(500*1000)	/* 500 ms */

/* Defined as sbh by bsp_config.h for convenience */
sb_t *bcm947xx_sbh = NULL;

/* Configured devices */
#if (CFG_FLASH || CFG_SFLASH) && CFG_XIP
#error "XIP and Flash cannot be defined at the same time"
#endif

extern cfe_driver_t ns16550_uart;
#if CFG_FLASH
extern cfe_driver_t newflashdrv;
#endif
#if CFG_SFLASH
extern cfe_driver_t sflashdrv;
#endif
#if CFG_ET
extern cfe_driver_t bcmet;
#endif
#if CFG_WL
extern cfe_driver_t bcmwl;
#endif
#if CFG_BCM57XX
extern cfe_driver_t bcm5700drv;
#endif

/* Reset NVRAM */
static int restore_defaults = 0;

static void
board_console_add(void *regs, uint irq, uint baud_base, uint reg_shift)
{
	physaddr_t base;

	/* The CFE NS16550 driver expects a physical address */
	base = PHYSADDR((physaddr_t) regs);
	cfe_add_device(&ns16550_uart, base, baud_base, &reg_shift);
}

#if CFG_FLASH || CFG_SFLASH
#if 0
static void
reset_release_wait (void)
{
	int gpio;

	if ((gpio = nvram_resetgpio_init ((void *)sbh)) < 0)
		return;

	while (1) {
		/* Reset button is active low */
		if (sb_gpioin(sbh) & ((uint32)1 << gpio)) {
			OSL_DELAY(RESET_DEBOUNCE_TIME);

			if (sb_gpioin(sbh) & ((uint32)1 << gpio))
				break;
		}
	} 
}
#endif
#endif
/*  *********************************************************************
    *  board_console_init()
    *  
    *  Add the console device and set it to be the primary
    *  console.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */
#if CFG_BUFLOG
extern int logbuf_write(const char *str);
extern int logbuf_dump(void);
#endif

void
board_console_init(void)
{
	unsigned int boardtype;
	unsigned long mipsclock = 0, sbclock = 0, pciclock = 0;
	char *clkfreq;

#if CFG_BUFLOG
	xprinthook = logbuf_write;
#endif
	/* Initialize SB access */
	sbh = sb_kattach(SB_OSH);
	ASSERT(sbh);

#if !CFG_SIM
	/* Figure out current MIPS clock speed */
	if ((cfe_cpu_speed = sb_cpu_clock(sbh)) == 0)
		cfe_cpu_speed = 100000000;

	/* Initialize NVRAM access */
	if (nvram_init((void *)sbh) > 0)
		restore_defaults = 1;

	if (!restore_defaults) {
		char *end;

		/* MIPS clock speed override */
		if ((clkfreq = nvram_get("clkfreq"))) {
			mipsclock = bcm_strtoul(clkfreq, &end, 0) * 1000000;
			if (*end == ',')
				sbclock = bcm_strtoul(++end, NULL, 0) * 1000000;
		} else if (sb_chip(sbh) == BCM4710_DEVICE_ID) {
			mipsclock = 124800000;

			/* Use a 25 MHz PCI clock on bcm94710r* boards to drive the RoboSwitch */
			boardtype = sb_boardtype(sbh);
			if ((boardtype == BCM94710R1_BOARD) || (boardtype == BCM94710R4_BOARD))
				pciclock = 25000000;
		}

		if (mipsclock) {
			/* Set current MIPS clock speed */
			sb_mips_setclock(sbh, mipsclock, sbclock, pciclock);
		}
	}
#endif


	/* Initialize clocks and interrupts */
	sb_mips_init(sbh, 0);

	/* Initialize UARTs */
	sb_serial_init(sbh, board_console_add);

	if (cfe_finddev("uart0")) {
		cfe_set_console("uart0");
#if CFG_BUFLOG
		logbuf_dump();
#endif
	}
}

#if CFG_FLASH || CFG_SFLASH
static void
flash_init(void)
{
	newflash_probe_t fprobe;
	chipcregs_t *cc = NULL;
	extifregs_t *extif = NULL;
	uint32 fltype, bootsz, *bisz;
	cfe_driver_t *drv;

	memset(&fprobe, 0, sizeof(fprobe));

	fprobe.flash_phys = SB_FLASH1;
	if ((cc = (chipcregs_t *) sb_setcore(sbh, SB_CC, 0))) {
		fltype = R_REG(NULL, &cc->capabilities) & CC_CAP_FLASH_MASK;
		fprobe.flash_phys = SB_FLASH2;
	} else if ((extif = sb_setcore(sbh, SB_EXTIF, 0)))
		fltype = PFLASH;
	else
		fltype = FLASH_NONE;

	switch (fltype) {
#if CFG_FLASH
	case PFLASH:
		drv = &newflashdrv;
		fprobe.flash_flags = FLASH_FLG_BUS16 | FLASH_FLG_DEV16;
		if ((cc && !(R_REG(NULL, &cc->flash_config) & CC_CFG_DS)) ||
		    (extif && !(R_REG(NULL, &extif->flash_config) & CF_DS)))
			fprobe.flash_flags = FLASH_FLG_BUS8 | FLASH_FLG_DEV16;
		break;
#endif
#if CFG_SFLASH
	case SFLASH_ST:
	case SFLASH_AT:
		ASSERT(cc);
		drv = &sflashdrv;
		/* Overload cmdset field */
		fprobe.flash_cmdset = (int)cc;
		break;
#endif
	default:
		/* No flash or unsupported flash */
		return;
	}

	/* Default is 256K boot partition */
	bootsz = 256 * 1024;
	/* Do we have a self-describing binary image? */
	bisz = (uint32 *)PHYS_TO_K1(fprobe.flash_phys + BISZ_OFFSET);
	if (bisz[BISZ_MAGIC_IDX] == BISZ_MAGIC) {
		if ((bisz[BISZ_DATAEND_IDX] - bisz[BISZ_TXTST_IDX]) < (128 * 1024))
			bootsz = 128 * 1024;
		printf("Boot partition size = %d(0x%x)\n", bootsz, bootsz);
	}

	/* Because CFE can only boot from the beginning of a partition */
	fprobe.flash_nparts = 4;
	fprobe.flash_parts[0].fp_size = bootsz;
	fprobe.flash_parts[0].fp_name = "boot";
	fprobe.flash_parts[1].fp_size = sizeof(struct trx_header);
	fprobe.flash_parts[1].fp_name = "trx";
	fprobe.flash_parts[2].fp_size = 0;
	fprobe.flash_parts[2].fp_name = "os";
	fprobe.flash_parts[3].fp_size = NVRAM_SPACE;
	fprobe.flash_parts[3].fp_name = "nvram";
	cfe_add_device(drv, 0, 0, &fprobe);

	/* Because CFE can only flash an entire partition */
	fprobe.flash_nparts = 3;
	fprobe.flash_parts[0].fp_size = bootsz;
	fprobe.flash_parts[0].fp_name = "boot";
	fprobe.flash_parts[1].fp_size = 0;
	fprobe.flash_parts[1].fp_name = "trx";
	fprobe.flash_parts[2].fp_size = NVRAM_SPACE;
	fprobe.flash_parts[2].fp_name = "nvram";
	cfe_add_device(drv, 0, 0, &fprobe);

	/* Because sometimes we want to program the entire device */
	fprobe.flash_nparts = 0;
	cfe_add_device(drv, 0, 0, &fprobe);
}
#endif

/*  *********************************************************************
    *  board_device_init()
    *  
    *  Initialize and add other devices.  Add everything you need
    *  for bootstrap here, like disk drives, flash memory, UARTs,
    *  network controllers, etc.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_device_init(void)
{
	unsigned int unit;
#if CFG_ET || CFG_WL || CFG_BCM57XX
	void *regs;
#endif

	/* Set by board_console_init() */
	ASSERT(sbh);

#ifdef BCM4710
	if (sb_chip(sbh) == BCM4710_DEVICE_ID) {
		pcireg_t cr;

		/* Initialize PCI access */
		sbpci_init(sbh);

		/* Check PCI revision ID through external PCI config access */
		if (sbpci_read_config(sbh, 1, 0, 0, PCI_CLASS_REG, &cr, sizeof(cr)) == 0 &&
		    PCI_REVISION(cr) == 0)
			sbpci_check(sbh);
	}
#endif

#if CFG_FLASH || CFG_SFLASH
	flash_init();
#endif

	for (unit = 0; unit < SB_MAXCORES; unit++) {
#if CFG_ET
		if ((regs = sb_setcore(sbh, SB_ENET, unit)))
			cfe_add_device(&bcmet, BCM47XX_ENET_ID, unit, regs);
#endif
#if CFG_WL
		if ((regs = sb_setcore(sbh, SB_D11, unit)))
			cfe_add_device(&bcmwl, BCM4306_D11G_ID, unit, regs);
#endif
#if CFG_BCM57XX
		if((regs = sb_setcore(sbh, SB_GIGETH, unit)))
			cfe_add_device(&bcm5700drv, BCM47XX_GIGETH_ID, unit, regs);
#endif
	}
}

/*  *********************************************************************
    *  board_device_reset()
    *  
    *  Reset devices.  This call is done when the firmware is restarted,
    *  as might happen when an operating system exits, just before the
    *  "reset" command is applied to the installed devices.   You can
    *  do whatever board-specific things are here to keep the system
    *  stable, like stopping DMA sources, interrupts, etc.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_device_reset(void)
{
}

/*  *********************************************************************
    *  board_final_init()
    *  
    *  Do any final initialization, such as adding commands to the
    *  user interface.
    *
    *  If you don't want a user interface, put the startup code here.  
    *  This routine is called just before CFE starts its user interface.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_final_init(void)
{
	char *addr, *mask, *wait_time;
	char buf[512], *cur = buf;
	int commit;
	uint32 ncdl;
#if CFG_WL && CFG_WLU && CFG_SIM
	char *ssid;
#endif

	ui_init_bcm947xxcmds();

	/* Force commit of embedded NVRAM */
	commit = restore_defaults;

	/* Set the SDRAM NCDL value into NVRAM if not already done */
	if ((getintvar(NULL, "sdram_ncdl") == 0) &&
	    ((ncdl = sb_memc_get_ncdl(sbh)) != 0)) {
		sprintf(buf, "0x%x", ncdl);
		nvram_set("sdram_ncdl", buf);
		commit = 1;
	}

	/* Set the bootloader version string if not already done */
	sprintf(buf, "CFE %s", EPI_ROUTER_VERSION_STR);
	if (strcmp(nvram_safe_get("pmon_ver"), buf) != 0) {
		nvram_set("pmon_ver", buf);
		commit = 1;
	}

#if CFG_FLASH || CFG_SFLASH
#if !CFG_SIM
	/* Commit NVRAM */
	if (commit) {
		printf("Committing NVRAM...");
		nvram_commit();
		printf("done\n");
// Mark by SJ_Yen
#if 0
		if (restore_defaults) {
			printf ("Waiting for reset button release...");
			reset_release_wait ();
			printf("done\n");
		}
#endif
	}
#endif

#if !CFG_SIM
	/* Reboot after restoring defaults */
	if (restore_defaults)
		sb_watchdog(sbh, 1);
#endif	/* !CFG_SIM */
#else
	if (commit)
		printf("Flash not configured, not commiting NVRAM...\n");
#endif

	/*
	 * Read the wait_time NVRAM variable and set the tftp max retries.
	 * Assumption: tftp_rrq_timeout & tftp_recv_timeout are set to 1sec.
	 */
	if ((wait_time = nvram_get("wait_time")) != NULL) {
		tftp_max_retries = atoi(wait_time);
		if (tftp_max_retries > MAX_WAIT_TIME)
			tftp_max_retries = MAX_WAIT_TIME;
		else if (tftp_max_retries < MIN_WAIT_TIME)
			tftp_max_retries = MIN_WAIT_TIME;
	}

	/* Configure network */
	if (cfe_finddev("eth0") &&
	    (addr = nvram_get("lan_ipaddr")) &&
	    (mask = nvram_get("lan_netmask"))) {
		sprintf(buf, "ifconfig eth0 -addr=%s -mask=%s", addr, mask);
		ui_docommand(buf);
#if CFG_WL && CFG_WLU && CFG_SIM
		if ((ssid = nvram_get("wl0_ssid"))) {
			sprintf(buf, "wl join %s", ssid);
			ui_docommand(buf);
		}
#endif
	}

#if !CFG_SIM
	/* Boot image */
	cur += sprintf(cur, "go;");
#endif	/* !CFG_SIM */

	/* Startup */
	if (cur > buf)
		env_setenv("STARTUP", buf, ENV_FLG_NORMAL);
}

