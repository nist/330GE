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

//
union ip2breadcrumb 
{
	struct { 
		unsigned char port, cat, codes, label;
	} __attribute__ ((packed)) hdr;
	unsigned long value;
};

#define ITRC_NO_PORT 	0xFF
#define CHANN	(pCh->port_index)

#define	ITRC_ERROR	'!'
#define	ITRC_INIT 	'A'
#define	ITRC_OPEN	'B'
#define	ITRC_CLOSE	'C'
#define	ITRC_DRAIN	'D'
#define	ITRC_IOCTL	'E'
#define	ITRC_FLUSH	'F'
#define	ITRC_STATUS	'G'
#define	ITRC_HANGUP	'H'
#define	ITRC_INTR 	'I'
#define	ITRC_SFLOW	'J'
#define	ITRC_SBCMD	'K'
#define	ITRC_SICMD	'L'
#define	ITRC_MODEM	'M'
#define	ITRC_INPUT	'N'
#define	ITRC_OUTPUT	'O'
#define	ITRC_PUTC	'P'
#define	ITRC_QUEUE	'Q'
#define	ITRC_STFLW	'R'
#define	ITRC_SFIFO	'S'
#define	ITRC_VERIFY	'V'
#define	ITRC_WRITE	'W'

#define	ITRC_ENTER	0x00
#define	ITRC_RETURN	0xFF

#define	ITRC_QUEUE_ROOM	2
#define	ITRC_QUEUE_CMD	6

