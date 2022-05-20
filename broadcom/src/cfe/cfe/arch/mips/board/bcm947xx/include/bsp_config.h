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
    *  BSP Configuration file			File: bsp_config.h
    *  
    *  This module contains global parameters and conditional
    *  compilation settings for building CFE.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001
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


#define CFG_INIT_L1		1	/* initialize the L1 cache */
#define CFG_INIT_L2		0	/* there is no L2 cache */

#define CFG_INIT_DRAM		1	/* initialize DRAM controller */
#define CFG_DRAM_SIZE		xxx	/* size of DRAM if you don't initialize */

#define CFG_NETWORK		1	/* define to include network support */
#define CFG_TCP			0	/* include TCP stack */

#define CFG_FATFS               0
#define CFG_HTTPFS		0	/* Enable HTTP filesystem */

#define CFG_UI			1	/* Define to enable user interface */

#define CFG_MULTI_CPUS		0	/* no multi-cpu support */

#ifndef CFG_HEAP_SIZE
#define CFG_HEAP_SIZE           400    /* heap size in kilobytes */
#endif

#define CFG_STACK_SIZE		8192	/* stack size (bytes, rounded up to K) */

#define CFG_SERIAL_BAUD_RATE	115200	/* normal console speed */

#define CFG_VENDOR_EXTENSIONS   0

/*
 * These parameters control the flash driver's sector buffer.  
 * If you write environment variables or make small changes to
 * flash sectors from user applications, you
 * need to have the heap big enough to store a temporary sector
 * for merging in small changes to flash sectors, so you
 * should set CFG_FLASH_ALLOC_SECTOR_BUFFER in that case.
 * Otherwise, you can provide an address in unallocated memory
 * of where to place the sector buffer.
 */
#define CFG_FLASH_ALLOC_SECTOR_BUFFER 0	/* '1' to allocate sector buffer from the heap */
#define CFG_FLASH_SECTOR_BUFFER_ADDR  (1*1024*1024-128*1024)
#define CFG_FLASH_SECTOR_BUFFER_SIZE  (128*1024)

/*
 * The flash staging buffer is where we store a flash image before we write
 * it to the flash.  It's too big for the heap. If zero then the code will
 * choose between the given ADDR and mem_topofmem, depending on which
 * area is bigger.
 */

#define CFG_FLASH_STAGING_BUFFER_ADDR (4096)
#define CFG_FLASH_STAGING_BUFFER_SIZE 0

/*
 * Default TFTP maximum number retries is 3.
 */

#define TFTP_MAX_RETRIES 3

