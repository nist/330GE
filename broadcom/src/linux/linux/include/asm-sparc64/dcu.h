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
/* $Id: dcu.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_DCU_H
#define _SPARC64_DCU_H

/* UltraSparc-III Data Cache Unit Control Register */
#define DCU_CP		0x0002000000000000 /* Physical Cache Enable w/o mmu*/
#define DCU_CV		0x0001000000000000 /* Virtual Cache Enable	w/o mmu	*/
#define DCU_ME		0x0000800000000000 /* NC-store Merging Enable	*/
#define DCU_RE		0x0000400000000000 /* RAW bypass Enable		*/
#define DCU_PE		0x0000200000000000 /* PCache Enable		*/
#define DCU_HPE		0x0000100000000000 /* HW prefetch Enable		*/
#define DCU_SPE		0x0000080000000000 /* SW prefetch Enable		*/
#define DCU_SL		0x0000040000000000 /* Secondary load steering Enab	*/
#define DCU_WE		0x0000020000000000 /* WCache enable		*/
#define DCU_PM		0x000001fe00000000 /* PA Watchpoint Byte Mask	*/
#define DCU_VM		0x00000001fe000000 /* VA Watchpoint Byte Mask	*/
#define DCU_PR		0x0000000001000000 /* PA Watchpoint Read Enable	*/
#define DCU_PW		0x0000000000800000 /* PA Watchpoint Write Enable	*/
#define DCU_VR		0x0000000000400000 /* VA Watchpoint Read Enable	*/
#define DCU_VW		0x0000000000200000 /* VA Watchpoint Write Enable	*/
#define DCU_DM		0x0000000000000008 /* DMMU Enable			*/
#define DCU_IM		0x0000000000000004 /* IMMU Enable			*/
#define DCU_DC		0x0000000000000002 /* Data Cache Enable		*/
#define DCU_IC		0x0000000000000001 /* Instruction Cache Enable	*/

#endif /* _SPARC64_DCU_H */
