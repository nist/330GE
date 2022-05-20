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
 * consolemap.h
 *
 * Interface between console.c, selection.c  and consolemap.c
 */
#define LAT1_MAP 0
#define GRAF_MAP 1
#define IBMPC_MAP 2
#define USER_MAP 3

struct vc_data;

extern unsigned char inverse_translate(struct vc_data *conp, int glyph);
extern unsigned short *set_translate(int m,int currcons);
extern int conv_uni_to_pc(struct vc_data *conp, long ucs);
