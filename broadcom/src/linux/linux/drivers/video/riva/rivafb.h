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
#ifndef __RIVAFB_H
#define __RIVAFB_H

#include <linux/config.h>
#include <linux/fb.h>
#include <video/fbcon.h>
#include <video/fbcon-cfb4.h>
#include <video/fbcon-cfb8.h>
#include <video/fbcon-cfb16.h>
#include <video/fbcon-cfb32.h>
#include "riva_hw.h"

/* GGI compatibility macros */
#define NUM_SEQ_REGS		0x05
#define NUM_CRT_REGS		0x41
#define NUM_GRC_REGS		0x09
#define NUM_ATC_REGS		0x15

/* holds the state of the VGA core and extended Riva hw state from riva_hw.c.
 * From KGI originally. */
struct riva_regs {
	u8 attr[NUM_ATC_REGS];
	u8 crtc[NUM_CRT_REGS];
	u8 gra[NUM_GRC_REGS];
	u8 seq[NUM_SEQ_REGS];
	u8 misc_output;
	RIVA_HW_STATE ext;
};

typedef struct {
	unsigned char red, green, blue, transp;
} riva_cfb8_cmap_t;

struct rivafb_info;
struct rivafb_info {
	struct fb_info info;	/* kernel framebuffer info */

	RIVA_HW_INST riva;	/* interface to riva_hw.c */

	const char *drvr_name;	/* Riva hardware board type */

	unsigned long ctrl_base_phys;	/* physical control register base addr */
	unsigned long fb_base_phys;	/* physical framebuffer base addr */

	caddr_t ctrl_base;	/* virtual control register base addr */
	caddr_t fb_base;	/* virtual framebuffer base addr */

	unsigned ram_amount;	/* amount of RAM on card, in bytes */
	unsigned dclk_max;	/* max DCLK */

	struct riva_regs initial_state;	/* initial startup video mode */
	struct riva_regs current_state;

	struct display disp;
	int currcon;
	struct display *currcon_display;

	struct rivafb_info *next;

	struct pci_dev *pd;	/* pointer to board's pci info */
	unsigned base0_region_size;	/* size of control register region */
	unsigned base1_region_size;	/* size of framebuffer region */

	struct riva_cursor *cursor;

	struct display_switch dispsw;

	riva_cfb8_cmap_t palette[256];	/* VGA DAC palette cache */

#if defined(FBCON_HAS_CFB16) || defined(FBCON_HAS_CFB32)
	union {
#ifdef FBCON_HAS_CFB16
		u_int16_t cfb16[16];
#endif
#ifdef FBCON_HAS_CFB32
		u_int32_t cfb32[16];
#endif
	} con_cmap;
#endif				/* FBCON_HAS_CFB16 | FBCON_HAS_CFB32 */
#ifdef CONFIG_MTRR
	struct { int vram; int vram_valid; } mtrr;
#endif
};

#endif /* __RIVAFB_H */
