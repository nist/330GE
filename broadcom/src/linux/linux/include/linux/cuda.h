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
 * Definitions for talking to the CUDA.  The CUDA is a microcontroller
 * which controls the ADB, system power, RTC, and various other things.
 *
 * Copyright (C) 1996 Paul Mackerras.
 */

/* CUDA commands (2nd byte) */
#define CUDA_WARM_START		0
#define CUDA_AUTOPOLL		1
#define CUDA_GET_6805_ADDR	2
#define CUDA_GET_TIME		3
#define CUDA_GET_PRAM		7
#define CUDA_SET_6805_ADDR	8
#define CUDA_SET_TIME		9
#define CUDA_POWERDOWN		0xa
#define CUDA_POWERUP_TIME	0xb
#define CUDA_SET_PRAM		0xc
#define CUDA_MS_RESET		0xd
#define CUDA_SEND_DFAC		0xe
#define CUDA_RESET_SYSTEM	0x11
#define CUDA_SET_IPL		0x12
#define CUDA_SET_AUTO_RATE	0x14
#define CUDA_GET_AUTO_RATE	0x16
#define CUDA_SET_DEVICE_LIST	0x19
#define CUDA_GET_DEVICE_LIST	0x1a
#define CUDA_GET_SET_IIC	0x22

#ifdef __KERNEL__

extern int find_via_cuda(void);
extern int via_cuda_start(void);
extern int cuda_request(struct adb_request *req,
			void (*done)(struct adb_request *), int nbytes, ...);
extern void cuda_poll(void);

#endif	/* __KERNEL */
