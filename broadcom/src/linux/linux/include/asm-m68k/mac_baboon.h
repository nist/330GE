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
 * Definitions for the "Baboon" custom IC on the PowerBook 190.
 */

#define BABOON_BASE (0x50F1A000)	/* same as IDE controller base */

#ifndef __ASSEMBLY__

struct baboon {
	char	pad1[208];	/* generic IDE registers, not used here */
	short	mb_control;	/* Control register:
				 * bit 5 : slot 2 power control
				 * bit 6 : slot 1 power control
				 */
	char	pad2[2];
	short	mb_status;	/* (0xD4) media bay status register:
				 *
				 * bit 0: ????
				 * bit 1: IDE interrupt active?
				 * bit 2: bay status, 0 = full, 1 = empty
				 * bit 3: ????
				 */
	char	pad3[2];	/* (0xD6) not used */
	short	mb_ifr;		/* (0xD8) media bay interrupt flags register:
				 *
				 * bit 0: ????
				 * bit 1: IDE controller interrupt
				 * bit 2: media bay status change interrupt
				 */
};

extern volatile struct baboon *baboon;

#endif /* __ASSEMBLY **/
