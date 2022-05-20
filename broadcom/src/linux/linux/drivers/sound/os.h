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
#define ALLOW_SELECT
#undef NO_INLINE_ASM
#define SHORT_BANNERS
#define MANUAL_PNP
#undef  DO_TIMINGS

#include <linux/module.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE > 131328
#define LINUX21X
#endif

#ifdef __KERNEL__
#include <linux/utsname.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/param.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <asm/page.h>
#include <asm/system.h>
#ifdef __alpha__
#include <asm/segment.h>
#endif
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/pci.h>
#endif

#include <linux/wrapper.h>
#include <linux/soundcard.h>

#define FALSE	0
#define TRUE	1

extern int sound_alloc_dma(int chn, char *deviceID);
extern int sound_open_dma(int chn, char *deviceID);
extern void sound_free_dma(int chn);
extern void sound_close_dma(int chn);

extern void reprogram_timer(void);

#define USE_AUTOINIT_DMA

extern caddr_t sound_mem_blocks[1024];
extern int sound_nblocks;

#undef PSEUDO_DMA_AUTOINIT
#define ALLOW_BUFFER_MAPPING

extern struct file_operations oss_sound_fops;
