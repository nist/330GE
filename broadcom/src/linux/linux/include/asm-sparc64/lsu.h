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
/* $Id: lsu.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_LSU_H
#define _SPARC64_LSU_H

/* LSU Control Register */
#define LSU_CONTROL_PM		0x000001fe00000000 /* Phys-watchpoint byte mask     */
#define LSU_CONTROL_VM		0x00000001fe000000 /* Virt-watchpoint byte mask     */
#define LSU_CONTROL_PR		0x0000000001000000 /* Phys-read watchpoint enable   */
#define LSU_CONTROL_PW		0x0000000000800000 /* Phys-write watchpoint enable  */
#define LSU_CONTROL_VR		0x0000000000400000 /* Virt-read watchpoint enable   */
#define LSU_CONTROL_VW		0x0000000000200000 /* Virt-write watchpoint enable  */
#define LSU_CONTROL_FM		0x00000000000ffff0 /* Parity mask enables.          */
#define LSU_CONTROL_DM		0x0000000000000008 /* Data MMU enable.              */
#define LSU_CONTROL_IM		0x0000000000000004 /* Instruction MMU enable.       */
#define LSU_CONTROL_DC		0x0000000000000002 /* Data cache enable.            */
#define LSU_CONTROL_IC		0x0000000000000001 /* Instruction cache enable.     */

#endif /* !(_SPARC64_LSU_H) */
