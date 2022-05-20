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
 * Linux Rendering Resource Manager
 *
 *          Implements the SGI-compatible rendering resource manager.
 *          This takes care of implementing the virtualized video hardware
 *          access required for OpenGL direct rendering.
 *
 * Author:  Miguel de Icaza (miguel@nuclecu.unam.mx)
 *
 * Fixes:
 */
#include <linux/module.h>

#include <asm/uaccess.h>
#include <asm/rrm.h>


int
rrm_open_rn (int rnid, void *arg)
{
	return 0;
}

int
rrm_close_rn (int rnid, void *arg)
{
	return 0;
}

int
rrm_bind_proc_to_rn (int rnid, void *arg)
{
	return 0;
}

typedef int (*rrm_function )(void *arg);

struct {
	int (*r_fn)(int rnid, void *arg);
	int arg_size;
} rrm_functions [] = {
	{ rrm_open_rn,         sizeof (struct RRM_OpenRN) },
	{ rrm_close_rn,        sizeof (struct RRM_CloseRN) },
	{ rrm_bind_proc_to_rn, sizeof (struct RRM_BindProcToRN) }
};

#define RRM_FUNCTIONS (sizeof (rrm_functions)/sizeof (rrm_functions [0]))

/* cmd is a number in the range [0..RRM_CMD_LIMIT-RRM_BASE] */
int
rrm_command (unsigned int cmd, void *arg)
{
	int i, rnid;

	if (cmd > RRM_FUNCTIONS){
		printk ("Called unimplemented rrm ioctl: %d\n", cmd + RRM_BASE);
		return -EINVAL;
	}
	i = verify_area (VERIFY_READ, arg, rrm_functions [cmd].arg_size);
	if (i) return i;

	if (__get_user (rnid, (int *) arg))
		return -EFAULT;
	return (*(rrm_functions [cmd].r_fn))(rnid, arg);
}

int
rrm_close (struct inode *inode, struct file *file)
{
	/* This routine is invoked when the device is closed */
	return 0;
}

EXPORT_SYMBOL(rrm_command);
EXPORT_SYMBOL(rrm_close);
