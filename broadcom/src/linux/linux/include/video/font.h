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
 *  font.h -- `Soft' font definitions
 *
 *  Created 1995 by Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#ifndef _VIDEO_FONT_H
#define _VIDEO_FONT_H

#include <linux/types.h>

struct fbcon_font_desc {
    int idx;
    char *name;
    int width, height;
    void *data;
    int pref;
};

#define VGA8x8_IDX	0
#define VGA8x16_IDX	1
#define PEARL8x8_IDX	2
#define VGA6x11_IDX	3
#define SUN8x16_IDX	4
#define SUN12x22_IDX	5
#define ACORN8x8_IDX	6

extern struct fbcon_font_desc	font_vga_8x8,
				font_vga_8x16,
				font_pearl_8x8,
				font_vga_6x11,
				font_sun_8x16,
				font_sun_12x22,
				font_acorn_8x8;

/* Find a font with a specific name */

extern struct fbcon_font_desc *fbcon_find_font(char *name);

/* Get the default font for a specific screen size */

extern struct fbcon_font_desc *fbcon_get_default_font(int xres, int yres);

/* Max. length for the name of a predefined font */
#define MAX_FONT_NAME	32

#endif /* _VIDEO_FONT_H */
