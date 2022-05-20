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
#ifndef __CRIS_SMPLOCK_H
#define __CRIS_SMPLOCK_H

#include <linux/config.h>

#ifdef CONFIG_SMP

#error "SMP is not supported for CRIS"

/*
 *	Locking the kernel 
 */
 
extern __inline void lock_kernel(void)
{
	unsigned long flags;
	int proc = smp_processor_id();

	save_flags(flags);
	cli();
	/* set_bit works atomic in SMP machines */
	while(set_bit(0, (void *)&kernel_flag)) 
	{
		/*
		 *	We just start another level if we have the lock 
		 */
		if (proc == active_kernel_processor)
			break;
		do 
		{
#ifdef __SMP_PROF__		
			smp_spins[smp_processor_id()]++;
#endif			
			/*
			 *	Doing test_bit here doesn't lock the bus 
			 */
			if (test_bit(proc, (void *)&smp_invalidate_needed))
				if (clear_bit(proc, (void *)&smp_invalidate_needed))
					local_flush_tlb();
		}
		while(test_bit(0, (void *)&kernel_flag));
	}
	/* 
	 *	We got the lock, so tell the world we are here and increment
	 *	the level counter 
	 */
	active_kernel_processor = proc;
	kernel_counter++;
	restore_flags(flags);
}

extern __inline void unlock_kernel(void)
{
	unsigned long flags;
	save_flags(flags);
	cli();
	/*
	 *	If it's the last level we have in the kernel, then
	 *	free the lock 
	 */
	if (kernel_counter == 0)
		panic("Kernel counter wrong.\n"); 
	
	if(! --kernel_counter) 
	{
		active_kernel_processor = NO_PROC_ID;
		clear_bit(0, (void *)&kernel_flag);
	}
	restore_flags(flags);
}

#endif
#endif
