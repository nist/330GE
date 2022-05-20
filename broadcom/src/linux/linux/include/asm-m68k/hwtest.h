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
/* Routines to test for presence/absence of hardware registers:
 * see arch/m68k/mm/hwtest.c.
 *  -- PMM <pmaydell@chiark.greenend.org.uk> 05/1998
 *
 * Removed __init from decls.  We might want them in modules, and 
 * the code is tiny anyway.  16/5/98 pb
 */

#ifndef __ASM_HWTEST_H
#define __ASM_HWTEST_H 

extern int hwreg_present(volatile void *regp);
extern int hwreg_write(volatile void *regp, unsigned short val);

#endif
