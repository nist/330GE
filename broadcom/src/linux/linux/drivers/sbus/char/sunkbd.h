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
/* $Id: sunkbd.h,v 1.1.1.1 2007/01/25 12:51:57 jiahao_jhou Exp $
 * sunkbd.h: Defines needed by SUN Keyboard drivers
 *
 * Copyright (C) 1997  Eddie C. Dost  (ecd@skynet.be)
 */

#ifndef _SPARC_SUNKBD_H
#define _SPARC_SUNKBD_H 1

#include <linux/config.h>

/* Keyboard defines for L1-A processing... */
#define SUNKBD_RESET		0xff
#define SUNKBD_L1		0x01
#define SUNKBD_UP		0x80
#define SUNKBD_A		0x4d

struct l1a_kbd_state {
	int kbd_id;
	int l1_down;
};

extern struct l1a_kbd_state l1a_state;

extern void keyboard_zsinit(void (*kbd_put_char)(unsigned char));
extern void sunkbd_inchar(unsigned char, struct pt_regs *);
extern void batten_down_hatches(void);

extern void sun_kbd_bh(unsigned long);
extern int sun_kbd_init(void);
extern void sun_compute_shiftstate(void);
extern void sun_setledstate(struct kbd_struct *, unsigned int);
extern unsigned char sun_getledstate(void);
extern int sun_setkeycode(unsigned int, unsigned int);
extern int sun_getkeycode(unsigned int);

#ifdef CONFIG_PCI

extern ushort *sun_key_maps[MAX_NR_KEYMAPS];
extern unsigned int sun_keymap_count;

extern char sun_func_buf[];
extern char *sun_func_table[MAX_NR_FUNC];
extern int sun_funcbufsize;
extern int sun_funcbufleft;

extern struct kbdiacr sun_accent_table[MAX_DIACR];
extern unsigned int sun_accent_table_size;

#endif /* CONFIG_PCI */

#endif /* !(_SPARC_SUNKBD_H) */
