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
 * BK Id: SCCS/s.oak_setup.h 1.5 05/17/01 18:14:21 cort
 */
/*
 *
 *    Copyright (c) 1999-2000 Grant Erickson <grant@lcse.umn.edu>
 *
 *    Module name: oak_setup.h
 *
 *    Description:
 *      Architecture- / platform-specific boot-time initialization code for
 *      the IBM PowerPC 403GCX "Oak" evaluation board. Adapted from original
 *      code by Gary Thomas, Cort Dougan <cort@cs.nmt.edu>, and Dan Malek
 *      <dan@netx4.com>.
 *
 */

#ifndef	__OAK_SETUP_H__
#define	__OAK_SETUP_H__

#include <asm/ptrace.h>
#include <asm/board.h>


#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char	 __res[sizeof(bd_t)];

extern void		 oak_init(unsigned long r3,
				  unsigned long ird_start,
				  unsigned long ird_end,
				  unsigned long cline_start,
				  unsigned long cline_end);
extern void		 oak_setup_arch(void);
extern int		 oak_setup_residual(char *buffer);
extern void		 oak_init_IRQ(void);
extern int		 oak_get_irq(struct pt_regs *regs);
extern void		 oak_restart(char *cmd);
extern void		 oak_power_off(void);
extern void		 oak_halt(void);
extern void		 oak_time_init(void);
extern int		 oak_set_rtc_time(unsigned long now);
extern unsigned long	 oak_get_rtc_time(void);
extern void		 oak_calibrate_decr(void);


#ifdef __cplusplus
}
#endif

#endif /* __OAK_SETUP_H__ */
