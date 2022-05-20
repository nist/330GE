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
 *      linux/drivers/video/pmag-ba-fb.h
 *
 *      TurboChannel PMAG-BA framebuffer card support,
 *      Copyright (C) 1999,2000,2001 by
 *      Michael Engel <engel@unix-ag.org>,
 *      Karsten Merker <merker@linuxtag.org>
 *      This file is subject to the terms and conditions of the GNU General
 *      Public License.  See the file COPYING in the main directory of this
 *      archive for more details.
 */

/*
 * Bt459 RAM DAC register base offset (rel. to TC slot base address)
 */

#define PMAG_BA_BT459_OFFSET                    0x00200000

/*
 * Begin of PMAG-BA framebuffer memory relative to TC slot address,
 * resolution is 1024x864x8
 */

#define PMAG_BA_ONBOARD_FBMEM_OFFSET    0x00000000
