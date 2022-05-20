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
#ifndef _SGI_GRAPHICS_H
#define _SGI_GRAPHICS_H

#define MAXCARDS 4

struct graphics_ops {
	/* SGIism: Board owner, gets the shmiq requests from the kernel */
	struct task_struct *g_owner;

	/* Last process that got the graphics registers mapped  */
	struct task_struct *g_user;

	/* Board info */
	void               *g_board_info;
	int                g_board_info_len;

	/* These point to hardware registers that should be mapped with
	 * GFX_ATTACH_BOARD and the size of the information pointed to
	 */
	unsigned long      g_regs;
	int                g_regs_size;

	void (*g_save_context)(void *);
	void (*g_restore_context)(void *);
	void (*g_reset_console)(void);
	int  (*g_ioctl)(int device, int cmd, unsigned long arg);
};

void shmiq_init (void);
void streamable_init (void);

#endif /* _SGI_GRAPHICS_H */
