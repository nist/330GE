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
#ifndef __reg_h__
#define __reg_h__

/*
 * Exception frame offsets.
 */
#define EF_V0		0
#define EF_T0		1
#define EF_T1		2
#define EF_T2		3
#define EF_T3		4
#define EF_T4		5
#define EF_T5		6
#define EF_T6		7
#define EF_T7		8
#define EF_S0		9
#define EF_S1		10
#define EF_S2		11
#define EF_S3		12
#define EF_S4		13
#define EF_S5		14
#define EF_S6		15
#define EF_A3		16
#define EF_A4		17
#define EF_A5		18
#define EF_T8		19
#define EF_T9		20
#define EF_T10		21
#define EF_T11		22
#define EF_RA		23
#define EF_T12		24
#define EF_AT		25
#define EF_SP		26
#define EF_PS		27
#define EF_PC		28
#define EF_GP		29
#define EF_A0		30
#define EF_A1		31
#define EF_A2		32

#define EF_SIZE		(33*8)
#define HWEF_SIZE	(6*8)		/* size of PAL frame (PS-A2) */

#define EF_SSIZE	(EF_SIZE - HWEF_SIZE)

/*
 * Map register number into core file offset.
 */
#define CORE_REG(reg, ubase) \
	(((unsigned long *)((unsigned long)(ubase)))[reg])

#endif /* __reg_h__ */
