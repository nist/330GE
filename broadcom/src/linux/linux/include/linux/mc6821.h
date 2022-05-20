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
#ifndef _MC6821_H_
#define _MC6821_H_

/*
 * This file describes the memery mapping of the MC6821 PIA.
 * The unions describe overlayed registers. Which of them is used is
 * determined by bit 2 of the corresponding control register.
 * this files expects the PIA_REG_PADWIDTH to be defined the numeric
 * value of the register spacing.
 *
 * Data came from MFC-31-Developer Kit (from Ralph Seidel,
 * zodiac@darkness.gun.de) and Motorola Data Sheet (from 
 * Richard Hirst, srh@gpt.co.uk)
 *
 * 6.11.95 copyright Joerg Dorchain (dorchain@mpi-sb.mpg.de)
 *
 */

#ifndef PIA_REG_PADWIDTH
#define PIA_REG_PADWIDTH 255
#endif

struct pia {
	union {
		volatile u_char pra;
		volatile u_char ddra;
	} ua;
	u_char pad1[PIA_REG_PADWIDTH];
	volatile u_char cra;
	u_char pad2[PIA_REG_PADWIDTH];
	union {
		volatile u_char prb;
		volatile u_char ddrb;
	} ub;
	u_char pad3[PIA_REG_PADWIDTH];
	volatile u_char crb;
	u_char pad4[PIA_REG_PADWIDTH];
};

#define ppra ua.pra
#define pddra ua.ddra
#define pprb ub.prb
#define pddrb ub.ddrb

#define PIA_C1_ENABLE_IRQ (1<<0)
#define PIA_C1_LOW_TO_HIGH (1<<1)
#define PIA_DDR (1<<2)
#define PIA_IRQ2 (1<<6)
#define PIA_IRQ1 (1<<7)

#endif
