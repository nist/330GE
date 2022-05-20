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
/* $Id: dummy.h,v 1.1.1.1 2007/01/25 12:51:57 jiahao_jhou Exp $
 * drivers/sbus/audio/dummy.h
 *
 * Copyright (C) 1998 Derrick J. Brashear (shadow@dementia.org)
 */

#ifndef _DUMMY_H_
#define _DUMMY_H_

#include <linux/types.h>
#include <linux/tqueue.h>

#define DUMMY_OUTFILE "/usr/tmp/dummy.au"

/* Our structure for each chip */

struct dummy_chip {
	struct audio_info perchip_info;
	unsigned int playlen;
	struct tq_struct tqueue;
};

#define DUMMY_MIN_ATEN     (0)
#define DUMMY_MAX_ATEN     (31)
#define DUMMY_MAX_DEV_ATEN (63)

#define DUMMY_MON_MIN_ATEN         (0)
#define DUMMY_MON_MAX_ATEN         (63)

#define DUMMY_DEFAULT_PLAYGAIN     (132)
#define DUMMY_DEFAULT_RECGAIN      (126)

#define DUMMY_MIN_GAIN     (0)
#define DUMMY_MAX_GAIN     (15)

#define DUMMY_PRECISION    (8)             /* # of bits/sample */
#define DUMMY_CHANNELS     (1)             /* channels/sample */

#define DUMMY_RATE   (8000)                /* default sample rate */

#endif /* _DUMMY_H_ */
