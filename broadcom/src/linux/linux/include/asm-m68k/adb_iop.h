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
 * ADB through the IOP
 * Written by Joshua M. Thompson
 */

/* IOP number and channel number for ADB */

#define ADB_IOP		IOP_NUM_ISM
#define ADB_CHAN	2

/* From the A/UX headers...maybe important, maybe not */

#define ADB_IOP_LISTEN	0x01
#define ADB_IOP_TALK	0x02
#define ADB_IOP_EXISTS	0x04
#define ADB_IOP_FLUSH	0x08
#define ADB_IOP_RESET	0x10
#define ADB_IOP_INT	0x20
#define ADB_IOP_POLL	0x40
#define ADB_IOP_UNINT	0x80

#define AIF_RESET	0x00
#define AIF_FLUSH	0x01
#define AIF_LISTEN	0x08
#define AIF_TALK	0x0C

/* Flag bits in struct adb_iopmsg */

#define ADB_IOP_EXPLICIT	0x80	/* nonzero if explicit command */
#define ADB_IOP_AUTOPOLL	0x40	/* auto/SRQ polling enabled    */
#define ADB_IOP_SRQ		0x04	/* SRQ detected                */
#define ADB_IOP_TIMEOUT		0x02	/* nonzero if timeout          */

#ifndef __ASSEMBLY__

struct adb_iopmsg {
	__u8 flags;		/* ADB flags         */
	__u8 count;		/* no. of data bytes */
	__u8 cmd;		/* ADB command       */
	__u8 data[8];		/* ADB data          */
	__u8 spare[21];		/* spare             */
};

#endif /* __ASSEMBLY__ */
