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
#ifndef _SGI_GCONSOLE_H
#define _SGI_GCONSOLE_H


/*
 * This is a temporary measure, we should eventually migrate to
 * Geert's generic graphic console code.
 */

#define cmapsz 8192
#define CHAR_HEIGHT  16

struct console_ops {
	void (*set_origin)(unsigned short offset);
	void (*hide_cursor)(void);
	void (*set_cursor)(int currcons);
	void (*get_scrmem)(int currcons);
	void (*set_scrmem)(int currcons, long offset);
	int  (*set_get_cmap)(unsigned char *arg, int set);
	void (*blitc)(unsigned short charattr, unsigned long addr);
	void (*memsetw)(void *s, unsigned short c, unsigned int count);
	void (*memcpyw)(unsigned short *to, unsigned short *from, unsigned int count);
};

void register_gconsole (struct console_ops *);

/* This points to the system console */
extern struct console_ops *gconsole;

extern void gfx_init (const char **name);

extern void __set_origin (unsigned short offset);
extern void hide_cursor (void);
extern unsigned char vga_font[];

extern void disable_gconsole (void);
extern void enable_gconsole (void);

#endif /* _SGI_GCONSOLE */
