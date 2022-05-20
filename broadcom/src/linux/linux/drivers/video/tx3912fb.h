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

#include <linux/config.h>

/*
 * Begin platform specific configurations
 */
#if defined(CONFIG_NINO_4MB) || defined(CONFIG_NINO_8MB)
#define FB_X_RES       240
#define FB_Y_RES       320
#if defined(CONFIG_FBCON_CFB4)
#define FB_BPP         4
#else
#if defined(CONFIG_FBCON_CFB2)
#define FB_BPP         2
#else
#define FB_BPP         1
#endif
#endif
#define FB_IS_GREY     1
#define FB_IS_INVERSE  0
#endif

#ifdef CONFIG_NINO_16MB
#define FB_X_RES       240
#define FB_Y_RES       320
#define FB_BPP         8
#define FB_IS_GREY     0
#define FB_IS_INVERSE  0
#endif

/*
 * Define virtual resolutions if necessary
 */
#ifndef FB_X_VIRTUAL_RES
#define FB_X_VIRTUAL_RES FB_X_RES
#endif
#ifndef FB_Y_VIRTUAL_RES
#define FB_Y_VIRTUAL_RES FB_Y_RES
#endif

/*
 * Framebuffer address and size
 */
u_long tx3912fb_paddr = 0;
u_long tx3912fb_vaddr = 0;
u_long tx3912fb_size = (FB_X_RES * FB_Y_RES * FB_BPP / 8);

/*
 * Framebuffer info structure
 */
static struct fb_var_screeninfo tx3912fb_info = {
	FB_X_RES, FB_Y_RES,
	FB_X_VIRTUAL_RES, FB_Y_VIRTUAL_RES,
	0, 0,
	FB_BPP, FB_IS_GREY,
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	0, FB_ACTIVATE_NOW,
	-1, -1, 0, 20000,
	64, 64, 32, 32, 64, 2,
	0, FB_VMODE_NONINTERLACED,
	{0,0,0,0,0,0}
};

/*
 * Framebuffer name
 */
static char TX3912FB_NAME[16] = "tx3912fb";
