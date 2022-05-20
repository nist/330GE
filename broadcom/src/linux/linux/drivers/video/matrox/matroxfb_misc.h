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
#ifndef __MATROXFB_MISC_H__
#define __MATROXFB_MISC_H__

#include "matroxfb_base.h"

/* also for modules */
int matroxfb_PLL_calcclock(const struct matrox_pll_features* pll, unsigned int freq, unsigned int fmax,
	unsigned int* in, unsigned int* feed, unsigned int* post);
static inline int PLL_calcclock(CPMINFO unsigned int freq, unsigned int fmax,
		unsigned int* in, unsigned int* feed, unsigned int* post) {
	return matroxfb_PLL_calcclock(&ACCESS_FBINFO(features.pll), freq, fmax, in, feed, post);
}

void matroxfb_createcursorshape(WPMINFO struct display* p, int vmode);
int matroxfb_vgaHWinit(WPMINFO struct my_timming* m, struct display* p);
void matroxfb_vgaHWrestore(WPMINFO2);
void matroxfb_fastfont_init(struct matrox_fb_info* minfo);
int matrox_text_loadfont(WPMINFO struct display* p);
int matroxfb_fastfont_tryset(WPMINFO struct display* p);
void matroxfb_read_pins(WPMINFO2);

#endif	/* __MATROXFB_MISC_H__ */
