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
#include <linux/init.h>

#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>

static struct fs_struct init_fs = INIT_FS;
static struct files_struct init_files = INIT_FILES;
static struct signal_struct init_signals = INIT_SIGNALS;
struct mm_struct init_mm = INIT_MM(init_mm);

/*
 * Initial task structure.
 *
 * We need to make sure that this is 16384-byte aligned due to the
 * way process stacks are handled. This is done by having a special
 * "init_task" linker map entry..
 */
unsigned char interrupt_stack[ISTACK_SIZE] __attribute__ ((section("init_istack"), aligned(4096)));
union task_union init_task_union 
	__attribute__((section("init_task"), aligned(4096))) = { INIT_TASK(init_task_union.task) };

pgd_t swapper_pg_dir[PTRS_PER_PGD] __attribute__ ((aligned(4096))) = { {0}, };
#ifdef __LP64__
unsigned long pmd0[PTRS_PER_PMD] __attribute__ ((aligned(4096))) = { 0, };
#endif
unsigned long pg0[PT_INITIAL * PTRS_PER_PTE] __attribute__ ((aligned(4096))) = { 0, };
