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
#include <linux/mm.h>
#include <linux/sched.h>

#include <asm/pgtable.h>
#include <asm/uaccess.h>

static struct fs_struct init_fs = INIT_FS;
static struct files_struct init_files = INIT_FILES;
static struct signal_struct init_signals = INIT_SIGNALS;
struct mm_struct init_mm = INIT_MM(init_mm);

/* .text section in head.S is aligned at 8k boundry and this gets linked
 * right after that so that the init_task_union is aligned properly as well.
 * If this is not aligned on a 8k boundry, then you should change code
 * in etrap.S which assumes it.
 */
__asm__(".section \".text\",#alloc\n");
union task_union init_task_union =
		{ INIT_TASK(init_task_union.task) };
