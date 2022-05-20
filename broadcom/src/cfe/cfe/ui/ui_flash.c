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
    *  Flash Update commands			File: ui_flash.c
    *  
    *  The routines in this file are used for updating the 
    *  flash with new firmware.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001,2002,2003
    *  ASUSTek Inc.. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions 
    *     as they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of ASUSTek Inc..  The "Broadcom Corporation" 
    *     name may not be used to endorse or promote products derived 
    *     from this software without the prior written permission of 
    *     ASUSTek Inc..
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

#include "cfe_iocb.h"
#include "cfe_devfuncs.h"
#include "cfe_ioctl.h"
#include "cfe_timer.h"
#include "cfe_error.h"

#include "ui_command.h"
#include "cfe.h"

#include "cfe_fileops.h"
#include "cfe_boot.h"
#include "bsp_config.h"

#include "cfe_loader.h"

#include "net_ebuf.h"
#include "net_ether.h"
#include "net_api.h"

#include "cfe_flashimage.h"

#include "addrspace.h"
#include "initdata.h"
#include "url.h"


/*  *********************************************************************
    *  Constants
    ********************************************************************* */

/*
 * Of course, these things really belong somewhere else.
 */

#define FLASH_STAGING_BUFFER	CFG_FLASH_STAGING_BUFFER_ADDR
#ifdef _FLASHPROG_
#define FLASH_STAGING_BUFFER_SIZE     (1024*1024*16)
#else
#define FLASH_STAGING_BUFFER_SIZE CFG_FLASH_STAGING_BUFFER_SIZE
#endif

// Added by PaN 2004/07/15
#define NVRAM_MAGIC                     0x48534C46      /* 'FLSH' */
#define NVRAM_MAGIC_MAC0                0x3043414D      /* 'MAC0' Added by PaN */
#define NVRAM_MAGIC_MAC1                0x3143414D      /* 'MAC1' Added by PaN */
#define NVRAM_MAGIC_RDOM                0x4D4F4452      /* 'RDOM' Added by PaN */
#define NVRAM_MAGIC_ASUS                0x53555341      /* 'ASUS' Added by PaN */
#define	NVRAM_MAGIC_SCODE		0x45444F4353	/* 'SCODE' Added by Yen */
extern int send_rescueack(unsigned short no, unsigned short lo);
// end PaN



/*  *********************************************************************
    *  Exerns
    ********************************************************************* */

extern int cfe_iocb_dispatch(cfe_iocb_t *iocb);

int ui_init_flashcmds(void);
static int ui_cmd_flash(ui_cmdline_t *cmd,int argc,char *argv[]);
// Added by Yen
void replace(uint8_t *bootbuf, char *keyword, uint8_t *value);
//end Yen


/*  *********************************************************************
    *  ui_init_flashcmds()
    *  
    *  Initialize the flash commands, add them to the table.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   0 if ok, else error
    ********************************************************************* */

int ui_init_flashcmds(void)
{
    cmd_addcmd("flash",
	       ui_cmd_flash,
	       NULL,
	       "Update a flash memory device",
#ifndef CFG_MINIMAL_SIZE 
	       "flash [options] filename [flashdevice]\n\n"
	       "Copies data from a source file name or device to a flash memory device.\n"
               "The source device can be a disk file (FAT filesystem), a remote file\n"
               "(TFTP) or a flash device.  The destination device may be a flash or eeprom.\n"
#if !CFG_EMBEDDED_PIC
	       "If the destination device is your boot flash (usually flash0), the flash\n"
	       "command will restart the firmware after the flash update is complete\n"
#endif
               "",
               "-noerase;Don't erase flash before writing|"
               "-offset=*;Begin programming at this offset in the flash device|"
               "-size=*;Size of source device when programming from flash to flash|"
	       "-noheader;Override header verification, flash binary without checking|"
	       "-mem;Use memory as source instead of a device"
#else /* CFG_MINIMAL_SIZE */
	       "flash [options] filename [flashdevice]\n",
               "-noerase;|"
               "-offset=*;|"
               "-size=*;|"
	       "-noheader;|"
	       "-mem;"
#endif /* CFG_MINIMAL_SIZE */
	       );


    return 0;
}

/*  *********************************************************************
    *  flash_crc32(buf,len)
    *  
    *  Yes, this is an Ethernet CRC.  I'm lazy.
    *  
    *  Input parameters: 
    *  	   buf - buffer to CRC
    *  	   len - length of data
    *  	   
    *  Return value:
    *  	   CRC-32
    ********************************************************************* */

#define     CRC32_POLY        0xEDB88320UL    /* CRC-32 Poly */
static unsigned int
flash_crc32(const unsigned char *databuf, unsigned int  datalen) 
{       
    unsigned int idx, bit, data, crc = 0xFFFFFFFFUL;
 
    for (idx = 0; idx < datalen; idx++) {
	for (data = *databuf++, bit = 0; bit < 8; bit++, data >>= 1) {
	    crc = (crc >> 1) ^ (((crc ^ data) & 1) ? CRC32_POLY : 0);
	    }
	}

    return crc;
}

/*  *********************************************************************
    *  flash_validate(ptr)
    *  
    *  Validate the flash header to make sure we can program it.
    *  
    *  Input parameters: 
    *  	   ptr - pointer to flash header
    *      outptr - pointer to data that we should program
    *	   outsize - size of data we should program
    *  	   
    *  Return value:
    *  	   0 if ok
    *  	   else error occured
    ********************************************************************* */

#define GET32(x) (((uint32_t) (x[0] << 24)) | \
                  ((uint32_t) (x[1] << 16)) | \
                  ((uint32_t) (x[2] << 8)) |  \
                  ((uint32_t) (x[3] << 0)))

static int flash_validate(uint8_t *ptr,int bufsize,int insize,uint8_t **outptr,int *outsize)
{
    cfe_flashimage_t *hdr = (cfe_flashimage_t *) ptr;
    uint32_t size;
    uint32_t flags;
    uint32_t hdrcrc;
    uint32_t calccrc;

    if (memcmp(hdr->seal,CFE_IMAGE_SEAL,sizeof(hdr->seal)) != 0) {
	printf("Invalid header seal.  This is not a CFE flash image.\n");
	return -1;
	}

    printf("Flash image contains CFE version %d.%d.%d for board '%s'\n",
	   hdr->majver,hdr->minver,hdr->ecover,hdr->boardname);

    size = GET32(hdr->size);
    flags = GET32(hdr->flags);
    hdrcrc = GET32(hdr->crc);
    printf("Flash image is %d bytes, flags %08X, CRC %08X\n",size,flags,hdrcrc);

    if (strcmp(CFG_BOARDNAME,hdr->boardname) != 0) {
	printf("This flash image is not appropriate for board type '%s'\n",CFG_BOARDNAME);
	return -1;
	}

    if ((size == 0) || (size > bufsize) ||
	((size + sizeof(cfe_flashimage_t)) < insize)) {
	printf("Flash image size is bogus!\n");
	return -1;
	}

    calccrc = flash_crc32(ptr + sizeof(cfe_flashimage_t),size);

    if (calccrc != hdrcrc) {
	printf("CRC is incorrect. Calculated CRC is %08X\n",calccrc);
	return -1;
	}

    *outptr = ptr + sizeof(cfe_flashimage_t);
    *outsize = size;
    return 0;
}


/*  *********************************************************************
    *  ui_get_flashbuf(bufptr, bufsize)
    *  
    *  Figure out the location and size of the staging buffer.
    *  
    *  Input parameters:
    *	   bufptr - address to return buffer location
    *	   bufsize - address to return buffer size
    ********************************************************************* */


static void ui_get_flash_buf(uint8_t **bufptr, int *bufsize)
{
    int size = FLASH_STAGING_BUFFER_SIZE;

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
	*bufptr = (uint8_t *) KERNADDR(FLASH_STAGING_BUFFER);
	*bufsize = size;
    } else {
	int below, above;

	below = PHYSADDR(mem_bottomofmem) - FLASH_STAGING_BUFFER;
	above = (mem_totalsize << 10) - PHYSADDR(mem_topofmem);

	if (below > above) {
	    *bufptr = (uint8_t *) KERNADDR(FLASH_STAGING_BUFFER);
	    *bufsize = below;
	} else {
	    *bufptr = (uint8_t *) KERNADDR(mem_topofmem);
	    *bufsize = above;
	}
    }
}


/*  *********************************************************************
    *  ui_cmd_flash(cmd,argc,argv)
    *  
    *  The 'flash' command lives here.  Program the boot flash,
    *  or if a device name is specified, program the alternate
    *  flash device.
    *  
    *  Input parameters: 
    *  	   cmd - command table entry
    *  	   argc,argv - parameters
    *  	   
    *  Return value:
    *  	   0 if ok
    *  	   else error
    ********************************************************************* */
// Yen
void replace(uint8_t *bootbuf, char *keyword, uint8_t *value)
{
        int addr=0, addr_i=0, addr_j=0, idx=0;
        int wordlen=strlen(keyword);
        char addr_buf[wordlen];
//	xprintf("Do Replace!\n");
        do
        {
                for(addr_i=0;addr_i<strlen(keyword);addr_i++)
                {
                        addr_buf[0x0+addr_i]=bootbuf[0x0000+addr+addr_i];
                }
                addr_buf[wordlen]=0x00;
                addr++;
//		xprintf("the keyword is %s, and the real one id %s\n", keyword, addr_buf);
        }
        while (!(strcmp(addr_buf, keyword)==0));
/*
	if(strcmp(keyword, "secret_code") == 0)
		idx = 8;
	else 
*/
	if (strcmp(keyword, "regulation_domain"))
		idx = 6;
	else
		idx = 17;

        for (addr_j=0 ; addr_j < idx ; addr_j++)
        {
//		xprintf("print out the bootbuf is %c and the MAC is %x", bootbuf[0x1000+addr+addr_j+strlen(keyword+1)], 
//		value[0x0+addr_j]);
	        bootbuf[0x0000+addr+addr_j+strlen(keyword)] = value[0x0+addr_j];
        }

}
// end Yen


// Modify by PaN 2004/07/15
static int ui_cmd_flash(ui_cmdline_t *cmd,int argc,char *argv[])
{
    uint8_t *ptr, bootbuf[0x40000], MAC0[18], RDOM[7], ASUS[24], nvramcmd[60], secretcode[9];
    int fh;
    int res;
#if !CFG_EMBEDDED_PIC
    int retlen;
#endif
    char *fname;
    char *flashdev;
    cfe_loadargs_t la;
    int amtcopy;
    int devtype;
    int srcdevtype;
    int chkheader;
    int sfd;
    int copysize;
    flash_info_t flashinfo;
    int offset = 0;
    int noerase = 0;
    int memsrc = 0; /* Source is memory address */
    char *x;
    int size = 0;
    int bufsize = 0;

	int i = 0, parseflag=0, j=0;
        char *et0mac="et0macaddr";
        char *reg_dom="regulation_domain";
        char *il0mac="il0macaddr";
	char *scode = "secret_code";
	uint8_t SCODE[5] = {0x53, 0x43, 0x4F, 0x44, 0x45};



    /* Get staging buffer */
    memsrc = cmd_sw_isset(cmd,"-mem");

    /* If memory is not being used as a source, then get staging buffer */
    if (!memsrc)
	    ui_get_flash_buf(&ptr, &bufsize);

    /*
     * Parse command line parameters
     */

    fname = cmd_getarg(cmd,0);

    if (!fname) {
	return ui_showusage(cmd);
	}

    flashdev = cmd_getarg(cmd,1);
    if (!flashdev) flashdev = "flash0.0";

    /*
     * Make sure it's a flash device.
     */

    res = cfe_getdevinfo(flashdev);
    if (res < 0) {
	return ui_showerror(CFE_ERR_DEVNOTFOUND,flashdev);
	}

    devtype = res & CFE_DEV_MASK;

    if ((res != CFE_DEV_FLASH) && (res != CFE_DEV_NVRAM)) {
	xprintf("Device '%s' is not a flash or eeprom device.\n",flashdev);
	return CFE_ERR_INV_PARAM;
	}

    /*
     * We shouldn't really allow this, but there are some circumstances
     * where you might want to bypass the header check and shoot
     * yourself in the foot.
     * Switch normally not supplied, so chkheader will be TRUE.
     */

    chkheader = !cmd_sw_isset(cmd,"-noheader");

    /*
     * Check for some obscure options here.
     */

    noerase = cmd_sw_isset(cmd,"-noerase");

    if (cmd_sw_value(cmd,"-offset",&x)) {
        offset = atoi(x);
        }

    if (cmd_sw_value(cmd,"-size",&x)) {
        size = atoi(x);
        }

    /* Fix up the ptr and size for reading from memory
     * and skip loading to go directly to programming
     */
    if (memsrc) {
	    ptr = (uint8_t *)xtoi(fname);
	    bufsize = size;
	    res = size;
	    xprintf("Reading from %s: ",fname);
	    goto program;
    }

    /*
     * Read the new flash image from the source device
     */

    srcdevtype = cfe_getdevinfo(fname) & CFE_DEV_MASK;

    xprintf("Reading %s: ",fname);

    switch (srcdevtype) {
	case CFE_DEV_FLASH:
	    sfd = cfe_open(fname);
	    if (sfd < 0) {
		return ui_showerror(sfd,"Could not open source device");
		}
	    memset(ptr,0xFF,bufsize);


	    if (cfe_ioctl(sfd,IOCTL_FLASH_GETINFO,
			  (unsigned char *) &flashinfo,
			  sizeof(flash_info_t),
			  &res,0) != 0) {
		flashinfo.flash_size = bufsize;
		}

	    if (size > 0) {
		xprintf("(size=0x%X) ",size);
		}
            else {
		size = flashinfo.flash_size;
		}

	    /* Make sure we don't overrun the staging buffer */
	    
	    if (size > bufsize) {
		size = bufsize;
		}

	    /* Read the flash device here. */

	    res = cfe_read(sfd,ptr,size);

	    cfe_close(sfd);
	    if (res < 0) {
		return ui_showerror(res,"Could not read from flash");
		}
	    chkheader = FALSE;		/* no header to check */
	    /* 
	     * Search for non-0xFF byte at the end.  This will work because
	     * flashes get erased to all FF's, we pre-fill our buffer to FF's,
	     */
	    while (res > 0) {
		if (ptr[res-1] != 0xFF) break;
		res--;
		}
	    break;	

	case CFE_DEV_SERIAL:
	    la.la_filesys = "raw";
	    la.la_filename = NULL;
	    la.la_device = fname;
	    la.la_address = (intptr_t) ptr;
	    la.la_options = 0;
	    la.la_maxsize = bufsize;
	    la.la_flags =  LOADFLG_SPECADDR;

	    res = cfe_load_program("srec",&la);
	
	    if (res < 0) {
		ui_showerror(res,"Failed.");
		return res;
		}
	    break;

	default:
		
	    res = ui_process_url(fname, cmd, &la);
	    if (res < 0) {
		ui_showerror(res,"Invalid file name %s",fname);
		return res;
		}

	    la.la_address = (intptr_t) ptr;
	    la.la_options = 0;
	    la.la_maxsize = bufsize;
	    la.la_flags =  LOADFLG_SPECADDR;

	    res = cfe_load_program("raw",&la);
	
	    if (res < 0) {
		ui_showerror(res,"Failed.");
		return res;
		}
	    break;

	}

    xprintf("Done. %d bytes read\n",res);

program:
    copysize = res;

        if (copysize>0 && copysize<=0x40000) {
                strcpy(flashdev, "flash0.boot");
                // copy bootcode to buffer
                for (i=0; i<0x40000; i++)
                        bootbuf[i] = (*(unsigned char *) (0xbfc00000+i));
                if ( (*(unsigned long *) (ptr+0x400) == NVRAM_MAGIC) && ( *(unsigned long *) (ptr) != NVRAM_MAGIC_MAC0)
                                && (*(unsigned long *) (ptr) != NVRAM_MAGIC_MAC1) && (*(unsigned long *) (ptr) != NVRAM_MAGIC_RDOM )
                                && (*(unsigned long *) (ptr) != NVRAM_MAGIC_ASUS )) {
                        xprintf("Download of 0x%x bytes completed\n", copysize);
                        xprintf("Write bootloader binary to FLASH (0xbfc00000)\n");
                        parseflag=1;
                }
                else if ( *(unsigned long *) (ptr) == NVRAM_MAGIC_MAC0 ) {
                        xprintf("Download of 0x%x bytes completed\n", copysize);
                        for (i=0; i<17; i++)
                                MAC0[i] = ptr[4+i];
                        MAC0[i]='\0';
			/* Wait for SCODE by SJ_Yen */
			i=i+4;
/* Debug message */
//			xprintf("ptr[1]=%x, SCODE[1]=%x\n", ptr[i], SCODE[0]);
//			xprintf("ptr[2]=%x, SCODE[2]=%x\n", ptr[i+1], SCODE[1]);
//			xprintf("ptr[3]=%x, SCODE[3]=%x\n", ptr[i+2], SCODE[2]);
//			xprintf("ptr[4]=%x, SCODE[4]=%x\n", ptr[i+3], SCODE[3]);
//			xprintf("ptr[5]=%x, SCODE[5]=%x\n", ptr[i+4], SCODE[4]);
			if ((ptr[i] == SCODE[0]) &&
				(ptr [i+1] == SCODE[1]) &&
				(ptr [i+2] == SCODE[2]) &&
				(ptr [i+3] == SCODE[3]) &&
				(ptr [i+4] == SCODE[4])	) {
//				xprintf("i=%d\n", i);
				for (i = 26 ; i < 34; i++) {
//				for (i = 26 ; i < 32; i++) {
					secretcode[j] = ptr [i];
					xprintf("secretcode[%d]=%x, ptr[%d]=%x\n", j, secretcode[j], i, ptr[i]);
					j++;
				}
//				secretcode[6]='0';
//				secretcode[7]='0';
				secretcode[j]='\0';
				xprintf("Write secret code = %s to FLASH\n", secretcode);
				replace(bootbuf, scode, secretcode);
			}
			/* End Yen */

                        replace(bootbuf, et0mac, MAC0);
                        replace(bootbuf, il0mac, MAC0);
			
                        xprintf("Write MAC0 = %s  to FLASH \n", MAC0);
//                        sprintf(nvramcmd, "nvram set et0macaddr=%s", MAC0);
//                        xprintf("%s\n", nvramcmd);
//                        ui_docommand(nvramcmd);
//                        ui_docommand("nvram commit");
                        parseflag=2;
                }
                else if ( *(unsigned long *) (ptr) == NVRAM_MAGIC_RDOM ) {
                        for (i=0; i<6; i++)
                                RDOM[i] = ptr[4+i];
                        RDOM[i] = '\0';
                        replace(bootbuf, reg_dom, RDOM);
                        xprintf("Write RDOM = %s  to FLASH \n", RDOM);
//                        sprintf(nvramcmd, "nvram set regulation_domain=%s", RDOM);
//                        xprintf("%s\n", nvramcmd);
//                        ui_docommand(nvramcmd);
//                        ui_docommand("nvram commit");
                        parseflag=3;
                }
                else if ( *(unsigned long *) (ptr) == NVRAM_MAGIC_ASUS ) {
                        for (i=0; i<23; i++)
                                ASUS[i] = ptr[4+i];
                        ASUS[i]='\0';
                        for (i=0; i<17; i++)
                                MAC0[i] = ASUS[i];
                        MAC0[i]='\0';
//Yen
                        replace(bootbuf, et0mac, MAC0);
                        replace(bootbuf, il0mac, MAC0);

                        for (i=0; i<6; i++)
                                RDOM[i] = ASUS[17+i];
                        RDOM[i] = '\0';
                        xprintf("Write MAC0 = %s  to FLASH \n", MAC0);
                        sprintf(nvramcmd, "nvram set et0macaddr=%s", MAC0);
                        xprintf("%s\n", nvramcmd);
                        ui_docommand(nvramcmd);

                        replace(bootbuf, reg_dom, RDOM);

                        xprintf("Write RDOM = %s  to FLASH \n", RDOM);
                        sprintf(nvramcmd, "nvram set regulation_domain=%s", RDOM);
                        xprintf("%s\n", nvramcmd);
//                        ui_docommand(nvramcmd);
//                        ui_docommand("nvram commit");
                        parseflag=4;
                }
                else {
                        parseflag=-1;
                        xprintf("Download of 0x%x bytes completed\n", copysize);
                        xprintf("What are you fucking doing!!\n");
                        copysize = 0;
                }
        }
        else if (copysize>0x40000) {
                parseflag=0;
                xprintf("Download of 0x%x bytes completed\n", copysize);
                xprintf("Write kernel and filesystem binary to FLASH (0xbfc40000)\n");
        }
        else {
                parseflag=-1;
                copysize = 0;
                xprintf("Downloading image time out\n");
        }


    /*
     * Verify the header and file's CRC.
     */
    if (chkheader) {
	if (flash_validate(ptr,bufsize,res,&ptr,&copysize) < 0) return -1;
	}

    if (copysize == 0) return 0;		/* 0 bytes, don't flash */

    /*
     * Open the destination flash device.
     */

    fh = cfe_open(flashdev);
    if (fh < 0) {
	xprintf("Could not open device '%s'\n",flashdev);
	return CFE_ERR_DEVNOTFOUND;
	}

    if (cfe_ioctl(fh,IOCTL_FLASH_GETINFO,
		  (unsigned char *) &flashinfo,
		  sizeof(flash_info_t),
		  &res,0) == 0) {
	/* Truncate write if source size is greater than flash size */
	if ((copysize + offset) > flashinfo.flash_size) {
            copysize = flashinfo.flash_size - offset;
	    }
	}

    /*
     * If overwriting the boot flash, we need to use the special IOCTL
     * that will force a reboot after writing the flash.
     */

    if (flashinfo.flash_flags & FLASH_FLAG_INUSE) {
#if CFG_EMBEDDED_PIC
	xprintf("\n\n** DO NOT TURN OFF YOUR MACHINE UNTIL THE FLASH UPDATE COMPLETES!! **\n\n");
#else
#if CFG_NETWORK
	if (net_getparam(NET_DEVNAME)) {
	    xprintf("Closing network.\n");
	    net_uninit();
	    }
#endif
	xprintf("Rewriting boot flash device '%s'\n",flashdev);
	xprintf("\n\n**DO NOT TURN OFF YOUR MACHINE UNTIL IT REBOOTS!**\n\n");
	cfe_ioctl(fh,IOCTL_FLASH_WRITE_ALL, ptr,copysize,&retlen,0);
	/* should not return */
	return CFE_ERR;
#endif
	}

    /*
     * Otherwise: it's not the flash we're using right
     * now, so we can be more verbose about things, and
     * more importantly, we can return to the command
     * prompt without rebooting!
     */

    /*
     * Erase the flash, if the device requires it.  Our new flash
     * driver does the copy/merge/erase for us.
     */

    if (!noerase) {
	if ((devtype == CFE_DEV_FLASH) && !(flashinfo.flash_flags & FLASH_FLAG_NOERASE)) {
	    flash_range_t range;
	    range.range_base = offset;
	    range.range_length = copysize;
	    xprintf("Erasing flash...");
	    if (cfe_ioctl(fh,IOCTL_FLASH_ERASE_RANGE,
			  (uint8_t *) &range,sizeof(range),NULL,0) != 0) {
		printf("Failed to erase the flash\n");
		cfe_close(fh);
		return CFE_ERR_IOERR;
		}
	    }
        }

    /*
     * Program the flash
     */
#if 0
    xprintf("Programming...");

    amtcopy = cfe_writeblk(fh,offset,ptr,copysize);

    if (copysize == amtcopy) {
	xprintf("done. %d bytes written\n",amtcopy);
	res = 0;
	}
    else {
	ui_showerror(amtcopy,"Failed.");
	res = CFE_ERR_IOERR;
	}
#endif
        if (parseflag != 0 && parseflag != 1) {
                for (i=0; i<0x40000; i++)
                        ptr[i] = bootbuf[i];
        }
        if (parseflag != 0)
                copysize = 0x40000;
        xprintf("Programming...\n");
        amtcopy = cfe_writeblk(fh,offset,ptr,copysize);
	xprintf("copysize=%d, amtcopy=%d \n", copysize, amtcopy);
                if (((copysize/2 == amtcopy) && (amtcopy <= 0x40000))
		  ||((copysize   == amtcopy) && (amtcopy > 0x40000))) {
                xprintf("done. %d bytes written\n",amtcopy);
                for (i=0; i<6; i++)
                                send_rescueack(0x0006, 0x0001);
                res = 0;
                if (parseflag != 0) {
                        printf("\nBCM47XX SYSTEM RESET!!!\n\n");
                        ui_docommand("reboot");
                }

        }
        else {
                ui_showerror(amtcopy,"Failed.");
                for (i=0; i<6; i++)
                        send_rescueack(0x0006, 0x0000);
                res = CFE_ERR_IOERR;
        }
    
    /* 	
     * done!
     */

    cfe_close(fh);

    return res;
}
//end PaN
// Added new command by PaN 2004/07/15
#if 0
static int ui_cmd_printdefault(ui_cmdline_t *cmd,int argc,char *argv[])
{
        int i;
        uint8_t bootbuf[0x40000];

        // copy bootcode to buffer
        for (i=0; i<0x40000; i++)
                bootbuf[i] = (*(unsigned char *) (0xbfc00000+i));
        for (i=0x1000; i<0x11B6; i++) {
                if (i>=0x1013 && bootbuf[i] == 0x0)
                        xprintf("\n");
                else
                        xprintf("%c", bootbuf[i]);
        }
        return 0;
}
#endif
// end PaN

