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
 * BK Id: SCCS/s.backlight.h 1.5 05/17/01 18:14:24 cort
 */
/*
 * Routines for handling backlight control on PowerBooks
 * 
 * For now, implementation resides in arch/ppc/kernel/pmac_support.c
 * 
 */
#ifdef __KERNEL__
#ifndef __ASM_PPC_BACKLIGHT_H
#define __ASM_PPC_BACKLIGHT_H

/* Abstract values */
#define BACKLIGHT_OFF	0
#define BACKLIGHT_MIN	1
#define BACKLIGHT_MAX	0xf

struct backlight_controller {
	int (*set_enable)(int enable, int level, void *data);
	int (*set_level)(int level, void *data);
};

extern void register_backlight_controller(struct backlight_controller *ctrler, void *data, char *type);
extern void unregister_backlight_controller(struct backlight_controller *ctrler, void *data);

extern int set_backlight_enable(int enable);
extern int get_backlight_enable(void);
extern int set_backlight_level(int level);
extern int get_backlight_level(void);

#endif
#endif /* __KERNEL__ */
