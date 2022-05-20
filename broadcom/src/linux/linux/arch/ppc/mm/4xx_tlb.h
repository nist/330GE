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
 * BK Id: SCCS/s.4xx_tlb.h 1.5 05/17/01 18:14:23 cort
 */
/*
 *
 *    Copyright (c) 1999 Grant Erickson <grant@lcse.umn.edu>
 *
 *    Module name: 4xx_tlb.h
 *
 *    Description:
 *      Routines for manipulating the TLB on PowerPC 400-class processors.
 *
 */

#ifndef __4XX_TLB_H__
#define __4XX_TLB_H__


#ifdef __cplusplus
extern "C" {
#endif


/* Function Prototypes */

extern void	 PPC4xx_tlb_pin(unsigned long va, unsigned long pa,
				int pagesz, int cache);
extern void	 PPC4xx_tlb_unpin(unsigned long va, unsigned long pa,
				  int size);
extern void	 PPC4xx_tlb_flush_all(void);
extern void	 PPC4xx_tlb_flush(unsigned long va, int pid);


#ifdef __cplusplus
}
#endif

#endif /* __4XX_TLB_H__ */
