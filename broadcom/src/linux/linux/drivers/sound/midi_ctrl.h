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
static unsigned char ctrl_def_values[128] =
{
	0x40,0x00,0x40,0x40,  0x40,0x40,0x40,0x7f,	/*   0 to   7 */
	0x40,0x40,0x40,0x7f,  0x40,0x40,0x40,0x40,	/*   8 to  15 */
	0x40,0x40,0x40,0x40,  0x40,0x40,0x40,0x40,	/*  16 to  23 */
	0x40,0x40,0x40,0x40,  0x40,0x40,0x40,0x40,	/*  24 to  31 */

	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  32 to  39 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  40 to  47 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  48 to  55 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  56 to  63 */
	
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  64 to  71 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  72 to  79 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  80 to  87 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/*  88 to  95 */

	0x00,0x00,0x7f,0x7f,  0x7f,0x7f,0x00,0x00,	/*  96 to 103 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/* 104 to 111 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/* 112 to 119 */
	0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,	/* 120 to 127 */
};