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
/* $Id: sunserial.h,v 1.1.1.1 2007/01/25 12:51:57 jiahao_jhou Exp $
 * sunserial.h: SUN serial driver infrastructure (including keyboards).
 *
 * Copyright (C) 1997  Eddie C. Dost  (ecd@skynet.be)
 */

#ifndef _SPARC_SUNSERIAL_H
#define _SPARC_SUNSERIAL_H 1

#include <linux/config.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/kbd_kern.h>
#include <linux/console.h>

struct initfunc {
	int		(*init) (void);
	struct initfunc *next;
};

struct sunserial_operations {
	struct initfunc	*rs_init;
	void		(*rs_kgdb_hook) (int);
	void		(*rs_change_mouse_baud) (int);
	int		(*rs_read_proc) (char *, char **, off_t, int, int *, void *);
};

struct sunkbd_operations {
	struct initfunc	*kbd_init;
	void		(*compute_shiftstate) (void);
	void		(*setledstate) (struct kbd_struct *, unsigned int);
	unsigned char	(*getledstate) (void);
	int		(*setkeycode) (unsigned int, unsigned int);
	int		(*getkeycode) (unsigned int);
};

extern struct sunserial_operations rs_ops;
extern struct sunkbd_operations kbd_ops;

extern void sunserial_setinitfunc(int (*) (void));
extern void sunkbd_setinitfunc(int (*) (void));

extern int serial_console;
extern int stop_a_enabled;
extern void sunserial_console_termios(struct console *);

#ifdef CONFIG_PCI
extern void sunkbd_install_keymaps(ushort **, unsigned int, char *,
				   char **, int, int, struct kbdiacr *,
				   unsigned int);
#endif

#endif /* !(_SPARC_SUNSERIAL_H) */
