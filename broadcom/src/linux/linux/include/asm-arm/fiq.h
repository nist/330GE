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
 *  linux/include/asm-arm/fiq.h
 *
 * Support for FIQ on ARM architectures.
 * Written by Philip Blundell <philb@gnu.org>, 1998
 * Re-written by Russell King
 */

#ifndef __ASM_FIQ_H
#define __ASM_FIQ_H

#include <asm/ptrace.h>

struct fiq_handler {
	struct fiq_handler *next;
	/* Name
	 */
	const char *name;
	/* Called to ask driver to relinquish/
	 * reacquire FIQ
	 * return zero to accept, or -<errno>
	 */
	int (*fiq_op)(void *, int relinquish);
	/* data for the relinquish/reacquire functions
	 */
	void *dev_id;
};

extern int claim_fiq(struct fiq_handler *f);
extern void release_fiq(struct fiq_handler *f);
extern void set_fiq_handler(void *start, unsigned int length);
extern void set_fiq_regs(struct pt_regs *regs);
extern void get_fiq_regs(struct pt_regs *regs);
extern void enable_fiq(int fiq);
extern void disable_fiq(int fiq);

#endif
