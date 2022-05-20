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
#ifndef __MATROXFB_TI3026_H__
#define __MATROXFB_TI3026_H__

/* make checkconfig does not walk through whole include tree */
#include <linux/config.h>

#include "matroxfb_base.h"

#ifdef CONFIG_FB_MATROX_MILLENIUM
extern struct matrox_switch matrox_millennium;
#endif

#endif	/* __MATROXFB_TI3026_H__ */
