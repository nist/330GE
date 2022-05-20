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
/* $Id: dcr.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_DCR_H
#define _SPARC64_DCR_H

/* UltraSparc-III/III+ Dispatch Control Register, ASR 0x12 */
#define DCR_DPE		0x0000000000001000 /* III+: D$ Parity Error Enable	*/
#define DCR_OBS		0x0000000000000fc0 /* Observability Bus Controls	*/
#define DCR_BPE		0x0000000000000020 /* Branch Predict Enable		*/
#define DCR_RPE		0x0000000000000010 /* Return Address Prediction Enable	*/
#define DCR_SI		0x0000000000000008 /* Single Instruction Disable	*/
#define DCR_IPE		0x0000000000000004 /* III+: I$ Parity Error Enable	*/
#define DCR_IFPOE	0x0000000000000002 /* IRQ FP Operation Enable		*/
#define DCR_MS		0x0000000000000001 /* Multi-Scalar dispatch		*/

#endif /* _SPARC64_DCR_H */
