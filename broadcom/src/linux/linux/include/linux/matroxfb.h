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
#ifndef __LINUX_MATROXFB_H__
#define __LINUX_MATROXFB_H__

#include <asm/ioctl.h>
#include <asm/types.h>

struct matroxioc_output_mode {
	__u32	output;		/* which output */
#define MATROXFB_OUTPUT_PRIMARY		0x0000
#define MATROXFB_OUTPUT_SECONDARY	0x0001
#define MATROXFB_OUTPUT_DFP		0x0002
	__u32	mode;		/* which mode */
#define MATROXFB_OUTPUT_MODE_PAL	0x0001
#define MATROXFB_OUTPUT_MODE_NTSC	0x0002
#define MATROXFB_OUTPUT_MODE_MONITOR	0x0080
};
#define MATROXFB_SET_OUTPUT_MODE	_IOW('n',0xFA,sizeof(struct matroxioc_output_mode))
#define MATROXFB_GET_OUTPUT_MODE	_IOWR('n',0xFA,sizeof(struct matroxioc_output_mode))

/* bitfield */
#define MATROXFB_OUTPUT_CONN_PRIMARY	(1 << MATROXFB_OUTPUT_PRIMARY)
#define MATROXFB_OUTPUT_CONN_SECONDARY	(1 << MATROXFB_OUTPUT_SECONDARY)
#define MATROXFB_OUTPUT_CONN_DFP	(1 << MATROXFB_OUTPUT_DFP)
/* connect these outputs to this framebuffer */
#define MATROXFB_SET_OUTPUT_CONNECTION	_IOW('n',0xF8,sizeof(__u32))
/* which outputs are connected to this framebuffer */
#define MATROXFB_GET_OUTPUT_CONNECTION	_IOR('n',0xF8,sizeof(__u32))
/* which outputs are available for this framebuffer */
#define MATROXFB_GET_AVAILABLE_OUTPUTS	_IOR('n',0xF9,sizeof(__u32))
/* which outputs exist on this framebuffer */
#define MATROXFB_GET_ALL_OUTPUTS	_IOR('n',0xFB,sizeof(__u32))

#endif

