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
    *  Test commands				File: cfe_tests.c
    *  
    *  A temporary sandbox for misc test routines and commands.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001
    *  ASUSTek Inc.. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accoddance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions as 
    *     they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of ASUSTek Inc.. Neither the "Broadcom 
    *     Corporation" name nor any trademark or logo of Broadcom 
    *     Corporation may be used to endorse or promote products 
    *     derived from this software without the prior written 
    *     permission of ASUSTek Inc..
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED 
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT, 
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */


#include "lib_types.h"
#include "lib_string.h"
#include "lib_queue.h"
#include "lib_malloc.h"
#include "lib_printf.h"
#include "cfe.h"
#include "cfe_iocb.h"
#include "cfe_devfuncs.h"
#include "cfe_error.h"
#include "cfe_fileops.h"
#include "cfe_loader.h"
#include "ui_command.h"
#include "bsp_config.h"

#include <typedefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <sbutils.h>
#include <sbconfig.h>
#include <sbchipc.h>
#include <sbmemc.h>
#include <bcmendian.h>
#include <bcmnvram.h>
#include <hndcpu.h>
#include <trxhdr.h>
#include "addrspace.h"
#include "initdata.h"

#include "bsp_priv.h"

/* Additional BSP UI */
#if CFG_ET
extern int ui_cmd_et(ui_cmdline_t *cmd, int argc, char *argv[]);
#endif

// Added by Yen 2006.09.25 
unsigned char DETECT(void);
bool mmode_set(void);
extern void LEDON(void);
extern void LEDOFF(void);
extern void GPIO_INIT(void);
// end Yen



static int
ui_cmd_reboot(ui_cmdline_t *cmd, int argc, char *argv[])
{
	hnd_cpu_reset(sbh);
	return 0;
}
#if 0
static int
ui_cmd_nvram(ui_cmdline_t *cmd, int argc, char *argv[])
{

	char *command, *name, *value; //, *buf;
	size_t size;
	int ret;

	if (!(command = cmd_getarg(cmd, 0)))
		return CFE_ERR_INV_PARAM;

	if (!strcmp(command, "get")) {
		if ((name = cmd_getarg(cmd, 1)))
			if ((value = nvram_get(name)))
				printf("%s\n", value);
	}
	else if (!strcmp(command, "set")){
		if ((name = cmd_getarg(cmd, 1))) {
			if ((value = strchr(name, '=')))
				*value++ = '\0';
			else if ((value = cmd_getarg(cmd, 2))) {
				if (*value == '=')
					value = cmd_getarg(cmd, 3);
			}
			if (value)
				nvram_set(name, value);
		}
	}
	else if (!strcmp(command, "unset")) {
		if ((name = cmd_getarg(cmd, 1)))
			nvram_unset(name);
	}
	else if (!strcmp(command, "commit")) {
		nvram_commit();
	}

	else if (!strcmp(command, "erase")) {
		if ((ret = cfe_open("flash0.nvram")) < 0)
			return ret;
		if (!(buf = KMALLOC(NVRAM_SPACE, 0)))
			return CFE_ERR_NOMEM;
		memset(buf, 0xff, NVRAM_SPACE);
		cfe_writeblk(ret, 0, buf, NVRAM_SPACE);
		cfe_close(ret);
		KFREE(buf);
	}

	else if (!strcmp(command, "show") || !strcmp(command, "getall")) {
		if (!(buf = KMALLOC(NVRAM_SPACE, 0)))
			return CFE_ERR_NOMEM;
		nvram_getall(buf, NVRAM_SPACE);
		for (name = buf; *name; name += strlen(name) + 1)
			printf("%s\n", name);
		size = sizeof(struct nvram_header) + (name - buf);
		printf("size: %d bytes (%d left)\n", size, NVRAM_SPACE - size);
		KFREE(buf);
	}

	return 0;
}
#endif


static int
check_trx(void)
{
	int ret;
	fileio_ctx_t *fsctx;
	void *ref;
	struct trx_header trx;
	uint32 crc, buf[128];
	unsigned int len, count;

	/* Open header */
	ret = fs_init("raw", &fsctx, "flash0.trx");
	if (ret)
		return ret;

	ret = fs_open(fsctx, &ref, "", FILE_MODE_READ);
	if (ret) {
		fs_uninit(fsctx);
		return ret;
	}

	/* Read header */
	ret = fs_read(fsctx, ref, (unsigned char *) &trx, sizeof(struct trx_header));
	if (ret != sizeof(struct trx_header)) {
		ret = CFE_ERR_IOERR;
		goto done;
	}
	
	/* Verify magic number */
	if (ltoh32(trx.magic) != TRX_MAGIC) {
		ret = CFE_ERR_INVBOOTBLOCK;
		goto done;
	}

	/* Checksum over header */
	crc = hndcrc32((uint8 *) &trx.flag_version,
		    sizeof(struct trx_header) - OFFSETOF(struct trx_header, flag_version),
		    CRC32_INIT_VALUE);

	fs_close(fsctx, ref);
	fs_uninit(fsctx);

	/* Open data */
	ret = fs_init("raw", &fsctx, "flash0.os");
	if (ret)
		return ret;

	ret = fs_open(fsctx, &ref, "", FILE_MODE_READ);
	if (ret) {
		fs_uninit(fsctx);
		return ret;
	}

	for (len = ltoh32(trx.len) - sizeof(struct trx_header); len; len -= count) {
		count = MIN(len, sizeof(buf));

		/* Read data */
		ret = fs_read(fsctx, ref, (unsigned char *) &buf, count);
		if (ret != count) {
			ret = CFE_ERR_IOERR;
			goto done;
		}

		/* Checksum over data */
		crc = hndcrc32((uint8 *) &buf, count, crc);
	}

	/* Verify checksum */
	if (ltoh32(trx.crc32) != crc) {
		ret = CFE_ERR_BOOTPROGCHKSUM;
		goto done;
	}

	ret = 0;

 done:
	fs_close(fsctx, ref);
	fs_uninit(fsctx);
	if (ret)
		xprintf("%s\n", cfe_errortext(ret));
	return ret;
}

// Added by PaN 2004/07/15
// Modify by Yen 2006.09.25
#define GPIO0   (1 << 0)
#define GPIO1   (1 << 1)
#define GPIO2   (1 << 2)
bool mmode_set(void)
{
//        void *sbh;
        unsigned long gpioin;

        sbh = sb_kattach(SB_OSH);
        ASSERT(sbh);
                gpioin = sb_gpioin(sbh);

        sb_detach(sbh);
        return gpioin & GPIO2 ? FALSE : TRUE;
}

extern void LEDON(void)
{
//        void *sbh;

        sbh = sb_kattach(SB_OSH);
        ASSERT(sbh);
        sb_gpioouten(sbh, GPIO0, GPIO0, GPIO_DRV_PRIORITY);

        /* led on */
        /* negative logic and hence val==0 */
        sb_gpioout(sbh, GPIO0, 0, GPIO_DRV_PRIORITY);
}

extern void GPIO_INIT(void) //Yen
{
//        void *sbh;

        sbh = sb_kattach(SB_OSH);
        ASSERT(sbh);
        sb_gpiocontrol(sbh, GPIO2, 0, GPIO_DRV_PRIORITY);
        sb_gpioouten(sbh, GPIO2, 0, GPIO_DRV_PRIORITY);
}


extern void LEDOFF(void)
{
//        void *sbh;

        sbh = sb_kattach(SB_OSH);
        ASSERT(sbh);
        sb_gpioouten(sbh, GPIO0, GPIO0, GPIO_DRV_PRIORITY);

        sb_gpioout(sbh, GPIO0, GPIO0, GPIO_DRV_PRIORITY);
}

unsigned char DETECT(void)
{
        unsigned char d = 0;
        char *rescueflag;

        if((rescueflag = nvram_get("rescueflag")) != NULL) {
                if (!nvram_invmatch("rescueflag", "enable")) {
                        xprintf("Rescue Flag enable.\n");
                        d = 1;
                }
                else {
                        xprintf("Rescue Flag disable.\n");
                        //d=(GPIOIN>>6)%2;
                        if (mmode_set())
                                d = 1;
                        else
                                d = 0;
                }
		nvram_set("rescueflag", "disable");
		nvram_commit();
//                ui_docommand("nvram set rescueflag=disable");
//                ui_docommand("nvram commit");

        }
        else {
                xprintf("Null Rescue Flag.\n");
                //d=(GPIOIN>>6)%2;
                if (mmode_set())
                        d = 1;
                else
                        d = 0;
        }
        if (d==1) // Set 1 to be high active and 0 to be low active
                return 1;
        else
                return 0;
}
// end PaN

// Modify by PaN 2004/07/15



/*  *********************************************************************
    *  ui_get_loadbuf(bufptr, bufsize)
    *  
    *  Figure out the location and size of the staging buffer.
    *  
    *  Input parameters:
    *	   bufptr - address to return buffer location
    *	   bufsize - address to return buffer size
    ********************************************************************* */
static void ui_get_loadbuf(uint8_t **bufptr, int *bufsize)
{
    int size = CFG_FLASH_STAGING_BUFFER_SIZE;

    /*	
     * Get the address of the staging buffer.  We can't
     * allocate the space from the heap to store the 
     * new flash image, because the heap may not be big
     * enough.  So, if FLASH_STAGING_BUFFER_SIZE is non-zero
     * then just use it and FLASH_STAGING_BUFFER; else
     * use the larger of (mem_bottomofmem - FLASH_STAGING_BUFFER)
     * and (mem_totalsize - mem_topofmem).
     */

    if (size > 0) {
	*bufptr = (uint8_t *) KERNADDR(CFG_FLASH_STAGING_BUFFER_ADDR);
	*bufsize = size;
    } else {
	int below, above;

	below = PHYSADDR(mem_bottomofmem) - CFG_FLASH_STAGING_BUFFER_ADDR;
	above = (mem_totalsize << 10) - PHYSADDR(mem_topofmem);

	if (below > above) {
	    *bufptr = (uint8_t *) KERNADDR(CFG_FLASH_STAGING_BUFFER_ADDR);
	    *bufsize = below;
	} else {
	    *bufptr = (uint8_t *) KERNADDR(mem_topofmem);
	    *bufsize = above;
	}
    }
}

// Modify by PaN
// Modify ui_cmd_go by Yen
static int
ui_cmd_go(ui_cmdline_t *cmd, int argc, char *argv[])
{
	int ret = 0;
	char buf[512];
	struct trx_header *file_buf;
	uint8_t *ptr;
	int bufsize = 0;
	int  i = 0;

        GPIO_INIT(); //Yen
        LEDON();

        if (DETECT()) {
                xprintf("Hello!! Enter Rescue Mode: (by Force)\n\n");
                /* Wait forever for an image */
                while ((ret = ui_docommand("flash -noheader : flash1.trx")) == CFE_ERR_TIMEOUT) {
                        if (i%2 == 0)
                                LEDON();
                        else
                                LEDOFF();
                        i++;
                        if (i==0xffffff)
                                i = 0;
                }
        }
        else {
                if (check_trx()) {
                        xprintf("Hello!! Enter Rescue Mode: (Check error)\n\n");
                        /* Wait forever for an image */
                        while ((ret = ui_docommand("flash -noheader : flash1.trx")) == CFE_ERR_TIMEOUT) {
                                if (i%2 == 0)
                                        LEDON();
                                else
                                        LEDOFF();
                                i++;
                                if (i==0xffffff)
                                        i = 0;
                        }
		} else if (!nvram_invmatch("boot_wait", "on")) {
			ui_get_loadbuf(&ptr, &bufsize);
			/* Load the image */
			sprintf(buf, "load -raw -addr=0x%x -max=0x%x :", ptr, TRX_MAX_LEN);
			ret = ui_docommand(buf);
	
			/* Load was successful. Check for the TRX magic.
			 * If it's a TRX image, then proceed to flash it, else try to boot
			 * Note: To boot a TRX image directly from the memory, address will need to be
			 * load address + trx header length.
			 */
			if (ret == 0) {
				file_buf = (struct trx_header *)ptr;
				/* If it's a TRX, then proceed to writing to flash else,
				 * try to boot from memory
				 */
				if (file_buf->magic != TRX_MAGIC) {
					sprintf(buf, "boot -raw -z -addr=0x%s -max=0x%x -fs=memory :0x%x",
					        nvram_get("os_ram_addr") ? : "80001000",
					        TRX_MAX_LEN, ptr);
					return ui_docommand(buf);
				}
				/* Flash the image from memory directly */
				sprintf(buf, "flash -noheader -mem -size=0x%x 0x%x flash1.trx",
			        	file_buf->len, ptr);
				ret = ui_docommand(buf);
			}
		}
	}

	if (ret == CFE_ERR_INTR)
		return ret;


	/* Boot the image */
	sprintf(buf, "boot -raw -z -addr=0x%s -max=0x%x flash0.os:",
		nvram_get("os_ram_addr") ? : "80001000",
		TRX_MAX_LEN);
	return ui_docommand(buf);
}


int
ui_init_bcm947xxcmds(void)
{
	cmd_addcmd("reboot",
		   ui_cmd_reboot,
		   NULL,
		   "Reboot.",
		   "reboot\n\n"
		   "Reboots.",
		   "");
#if 0
	cmd_addcmd("nvram",
		   ui_cmd_nvram,
		   NULL,
		   "NVRAM utility.",
#ifndef CFG_MINIMAL_SIZE 
		   "nvram [command] [args..]\n\n"
		   "Access NVRAM.",
		   "get [name];Gets the value of the specified variable|"
		   "set [name=value];Sets the value of the specified variable|"
		   "unset [name];Deletes the specified variable|"
		   "commit;Commit variables to flash|"
		   "erase;Erase all nvram|"
		   "show;Shows all variables|"
#else /* CFG_MINIMAL_SIZE */
		   "nvram [command] [args..]\n",
		   "get [name];|"
		   "set [name=value];|"
		   "unset [name];|"
		   "commit;|"
		   "erase;|"
		   "show;|"
#endif /* CFG_MINIMAL_SIZE */
		   );
#endif
	cmd_addcmd("go",
		   ui_cmd_go,
		   NULL,
		   "Verify and boot OS image.",
#ifndef CFG_MINIMAL_SIZE 
		   "go\n\n"
		   "Boots OS image if valid. Waits for a new OS image if image is invalid\n"
		   "or boot_wait is unset or not on.",
#else /* CFG_MINIMAL_SIZE */
		   "go\n",
#endif /* CFG_MINIMAL_SIZE */
		   "");
#if CFG_ET
	et_addcmd();
#endif
#if CFG_WLU
	wl_addcmd();
#endif

	return 0;
}
