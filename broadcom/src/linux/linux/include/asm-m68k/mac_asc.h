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
 *	Apple Sound Chip 
 */
 
#ifndef __ASM_MAC_ASC_H
#define __ASM_MAC_ASC_H

/*
 *	ASC offsets and controls
 */
 
#define ASC_BUF_BASE	0x00	/* RAM buffer offset */
#define ASC_BUF_SIZE	0x800

#define ASC_CONTROL	0x800
#define ASC_CONTROL_OFF		0x00
#define ASC_FREQ(chan,byte)	((0x810)+((chan)<<3)+(byte))
#define ASC_ENABLE	0x801
#define ASC_ENABLE_SAMPLE	0x02
#define ASC_MODE	0x802
#define ASC_MODE_SAMPLE		0x02

#define ASC_VOLUME	0x806
#define ASC_CHAN	0x807	/* ??? */

 
#endif
