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
 * Copyright (C) 2001 David J. Mckay (david.mckay@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.                            
 *
 * Defintions applicable to the STMicroelectronics ST40STB1 HARP and
 * compatible boards.
 */

#if defined(CONFIG_SH_STB1_HARP)

#define EPLD_BASE     0xa0800000

#define EPLD_LED      (EPLD_BASE+0x000c0000)
#define EPLD_INTSTAT0 (EPLD_BASE+0x00200000)
#define EPLD_INTSTAT1 (EPLD_BASE+0x00240000)
#define EPLD_INTMASK0 (EPLD_BASE+0x00280000)
#define EPLD_INTMASK1 (EPLD_BASE+0x002c0000)
#define EPLD_PAGEADDR (EPLD_BASE+0x00300000)
#define EPLD_REVID1   (EPLD_BASE+0x00380000)
#define EPLD_REVID2   (EPLD_BASE+0x003c0000)

#define EPLD_LED_ON  1
#define EPLD_LED_OFF 0

#elif defined(CONFIG_SH_STB1_OVERDRIVE)

#define EPLD_BASE     0xa7000000

#define EPLD_REVID    (EPLD_BASE+0x00000000)
#define EPLD_LED      (EPLD_BASE+0x00040000)
#define EPLD_INTMASK0 (EPLD_BASE+0x001c0000)
#define EPLD_INTMASK1 (EPLD_BASE+0x00200000)
#define EPLD_INTSTAT0 (EPLD_BASE+0x00240000)
#define EPLD_INTSTAT1 (EPLD_BASE+0x00280000)

#define EPLD_LED_ON  0
#define EPLD_LED_OFF 1

#else
#error Unknown board
#endif
