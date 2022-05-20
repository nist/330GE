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
#ifndef _LINUX_VIDEO_DECODER_H
#define _LINUX_VIDEO_DECODER_H

struct video_decoder_capability { /* this name is too long */
	__u32	flags;
#define	VIDEO_DECODER_PAL	1	/* can decode PAL signal */
#define	VIDEO_DECODER_NTSC	2	/* can decode NTSC */
#define	VIDEO_DECODER_SECAM	4	/* can decode SECAM */
#define	VIDEO_DECODER_AUTO	8	/* can autosense norm */
#define	VIDEO_DECODER_CCIR	16	/* CCIR-601 pixel rate (720 pixels per line) instead of square pixel rate */
	int	inputs;			/* number of inputs */
	int	outputs;		/* number of outputs */
};

/*
DECODER_GET_STATUS returns the following flags.  The only one you need is
DECODER_STATUS_GOOD, the others are just nice things to know.
*/
#define	DECODER_STATUS_GOOD	1	/* receiving acceptable input */
#define	DECODER_STATUS_COLOR	2	/* receiving color information */
#define	DECODER_STATUS_PAL	4	/* auto detected */
#define	DECODER_STATUS_NTSC	8	/* auto detected */
#define	DECODER_STATUS_SECAM	16	/* auto detected */


#define	DECODER_GET_CAPABILITIES _IOR('d', 1, struct video_decoder_capability)
#define	DECODER_GET_STATUS    	_IOR('d', 2, int)
#define	DECODER_SET_NORM	_IOW('d', 3, int)
#define	DECODER_SET_INPUT	_IOW('d', 4, int)	/* 0 <= input < #inputs */
#define	DECODER_SET_OUTPUT	_IOW('d', 5, int)	/* 0 <= output < #outputs */
#define	DECODER_ENABLE_OUTPUT	_IOW('d', 6, int)	/* boolean output enable control */
#define	DECODER_SET_PICTURE   	_IOW('d', 7, struct video_picture)

#define	DECODER_DUMP		_IO('d', 192)		/* debug hook */


#endif
