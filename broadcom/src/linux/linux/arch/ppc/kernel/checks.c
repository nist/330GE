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
 * BK Id: SCCS/s.checks.c 1.6 05/17/01 18:14:21 cort
 */
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/smp_lock.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/ptrace.h>
#include <linux/slab.h>
#include <linux/user.h>
#include <linux/a.out.h>

#include <asm/pgtable.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>

/*
 * Do various before compile checks of data structures
 * -- Cort
 */
int main(void)
{
	int ret = 0;

	if ( sizeof(struct pt_regs) % 16 )
	{
		printf("pt_regs struct is not modulo 16 bytes: "
			"%d bytes total, %d bytes off\n",
			sizeof(struct pt_regs),
			sizeof(struct pt_regs)%16);
		ret = -1;
		
	}

	printf("Task size        : %d bytes\n"
	       "Tss size         : %d bytes\n"
	       "pt_regs size     : %d bytes\n"
	       "Kernel stack size: %d bytes\n",
	       sizeof(struct task_struct), sizeof(struct thread_struct),
	       sizeof(struct pt_regs),
	       sizeof(union task_union) - sizeof(struct task_struct));
	return ret;
}
