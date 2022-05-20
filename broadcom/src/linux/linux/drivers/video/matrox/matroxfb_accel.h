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
#ifndef __MATROXFB_ACCEL_H__
#define __MATROXFB_ACCEL_H__

#include "matroxfb_base.h"

void matrox_init_putc(WPMINFO struct display* p, void (*)(WPMINFO struct display *p));
void matrox_cfbX_init(WPMINFO struct display* p);
void matrox_text_createcursor(WPMINFO struct display* p);
void matrox_text_round(CPMINFO struct fb_var_screeninfo* var, struct display* p);
void initMatrox(WPMINFO struct display* p);

#endif
