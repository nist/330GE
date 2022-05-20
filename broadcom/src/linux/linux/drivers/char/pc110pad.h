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
#ifndef _PC110PAD_H
#define _PC110PAD_H

#include <linux/ioctl.h>

enum pc110pad_mode {
	PC110PAD_RAW,		/* bytes as they come out of the hardware */
	PC110PAD_RARE,		/* debounced up/down and absolute x,y */
	PC110PAD_DEBUG,		/* up/down, debounced, transitions, button */
	PC110PAD_PS2,		/* ps2 relative (default) */ 
};


struct pc110pad_params {
	enum pc110pad_mode mode;
	int	bounce_interval;
	int	tap_interval;
	int	irq;
	int	io;
};

#define MS *HZ/1000

/* Appears as device major=10 (MISC), minor=PC110_PAD */

#define PC110PAD_IOCTL_TYPE		0x9a

#define PC110PADIOCGETP _IOR(PC110PAD_IOCTL_TYPE, 0, struct pc110pad_params)
#define PC110PADIOCSETP _IOW(PC110PAD_IOCTL_TYPE, 1, struct pc110pad_params)
 
#endif /* _PC110PAD_H */
