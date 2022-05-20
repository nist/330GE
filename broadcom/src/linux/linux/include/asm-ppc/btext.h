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
 * BK Id: %F% %I% %G% %U% %#%
 */
/*
 * Definitions for using the procedures in btext.c.
 *
 * Benjamin Herrenschmidt <benh@kernel.crashing.org>
 */
#ifndef __PPC_BTEXT_H
#define __PPC_BTEXT_H
#ifdef __KERNEL__

#include <asm/bootx.h>

extern void btext_clearscreen(void);
extern void btext_flushscreen(void);

extern unsigned long disp_BAT[2];

extern boot_infos_t disp_bi;
extern int boot_text_mapped;

void btext_init(boot_infos_t *bi);
void btext_welcome(void);
void btext_prepare_BAT(void);
void btext_setup_display(int width, int height, int depth, int pitch,
			 unsigned long address);
void map_boot_text(void);
void btext_update_display(unsigned long phys, int width, int height,
			  int depth, int pitch);

void btext_drawchar(char c);
void btext_drawstring(const char *str);
void btext_drawhex(unsigned long v);

#endif /* __KERNEL__ */
#endif /* __PPC_BTEXT_H */
