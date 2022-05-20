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
 * linux/drivers/char/busmouse.h
 *
 * Copyright (C) 1995 - 1998 Russell King
 *
 * Prototypes for generic busmouse interface
 */
#ifndef BUSMOUSE_H
#define BUSMOUSE_H

struct busmouse {
	int minor;
	const char *name;
	struct module *owner;
	int (*open)(struct inode * inode, struct file * file);
	int (*release)(struct inode * inode, struct file * file);
	int init_button_state;
};

extern void busmouse_add_movementbuttons(int mousedev, int dx, int dy, int buttons);
extern void busmouse_add_movement(int mousedev, int dx, int dy);
extern void busmouse_add_buttons(int mousedev, int clear, int eor);

extern int register_busmouse(struct busmouse *ops);
extern int unregister_busmouse(int mousedev);

#endif
