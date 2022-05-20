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
/* noquot.c: Quota stubs necessary for when quotas are not
 *           compiled into the kernel.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>

int nr_dquots, nr_free_dquots;
int max_dquots;

asmlinkage long sys_quotactl(int cmd, const char *special, int id, caddr_t addr)
{
	return(-ENOSYS);
}
