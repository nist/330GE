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
 *  FBcon low-level driver for 4 bpp packed pixel (cfb4)
 */

#ifndef _VIDEO_FBCON_CFB4_H
#define _VIDEO_FBCON_CFB4_H

#include <linux/config.h>

#ifdef MODULE
#if defined(CONFIG_FBCON_CFB4) || defined(CONFIG_FBCON_CFB4_MODULE)
#define FBCON_HAS_CFB4
#endif
#else
#if defined(CONFIG_FBCON_CFB4)
#define FBCON_HAS_CFB4
#endif
#endif

extern struct display_switch fbcon_cfb4;
extern void fbcon_cfb4_setup(struct display *p);
extern void fbcon_cfb4_bmove(struct display *p, int sy, int sx, int dy, int dx,
			     int height, int width);
extern void fbcon_cfb4_clear(struct vc_data *conp, struct display *p, int sy,
			     int sx, int height, int width);
extern void fbcon_cfb4_putc(struct vc_data *conp, struct display *p, int c,
			    int yy, int xx);
extern void fbcon_cfb4_putcs(struct vc_data *conp, struct display *p,
			     const unsigned short *s, int count, int yy, int xx);
extern void fbcon_cfb4_revc(struct display *p, int xx, int yy);

#endif /* _VIDEO_FBCON_CFB4_H */
