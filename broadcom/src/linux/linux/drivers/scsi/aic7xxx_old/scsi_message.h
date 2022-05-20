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
/* Messages (1 byte) */		     /* I/T (M)andatory or (O)ptional */
#define MSG_CMDCOMPLETE		0x00 /* M/M */
#define MSG_EXTENDED		0x01 /* O/O */
#define MSG_SAVEDATAPOINTER	0x02 /* O/O */
#define MSG_RESTOREPOINTERS	0x03 /* O/O */
#define MSG_DISCONNECT		0x04 /* O/O */
#define MSG_INITIATOR_DET_ERR	0x05 /* M/M */
#define MSG_ABORT		0x06 /* O/M */
#define MSG_MESSAGE_REJECT	0x07 /* M/M */
#define MSG_NOOP		0x08 /* M/M */
#define MSG_PARITY_ERROR	0x09 /* M/M */
#define MSG_LINK_CMD_COMPLETE	0x0a /* O/O */
#define MSG_LINK_CMD_COMPLETEF	0x0b /* O/O */
#define MSG_BUS_DEV_RESET	0x0c /* O/M */
#define MSG_ABORT_TAG		0x0d /* O/O */
#define MSG_CLEAR_QUEUE		0x0e /* O/O */
#define MSG_INIT_RECOVERY	0x0f /* O/O */
#define MSG_REL_RECOVERY	0x10 /* O/O */
#define MSG_TERM_IO_PROC	0x11 /* O/O */

/* Messages (2 byte) */
#define MSG_SIMPLE_Q_TAG	0x20 /* O/O */
#define MSG_HEAD_OF_Q_TAG	0x21 /* O/O */
#define MSG_ORDERED_Q_TAG	0x22 /* O/O */
#define MSG_IGN_WIDE_RESIDUE	0x23 /* O/O */

/* Identify message */		     /* M/M */	
#define MSG_IDENTIFYFLAG	0x80 
#define MSG_IDENTIFY_DISCFLAG	0x40 
#define MSG_IDENTIFY(lun, disc)	(((disc) ? 0xc0 : MSG_IDENTIFYFLAG) | (lun))
#define MSG_ISIDENTIFY(m)	((m) & MSG_IDENTIFYFLAG)

/* Extended messages (opcode and length) */
#define MSG_EXT_SDTR		0x01
#define MSG_EXT_SDTR_LEN	0x03

#define MSG_EXT_WDTR		0x03
#define MSG_EXT_WDTR_LEN	0x02
#define MSG_EXT_WDTR_BUS_8_BIT	0x00
#define MSG_EXT_WDTR_BUS_16_BIT	0x01
#define MSG_EXT_WDTR_BUS_32_BIT	0x02

#define MSG_EXT_PPR     0x04
#define MSG_EXT_PPR_LEN	0x06
#define MSG_EXT_PPR_OPTION_ST 0x00
#define MSG_EXT_PPR_OPTION_DT_CRC 0x02
#define MSG_EXT_PPR_OPTION_DT_UNITS 0x03
#define MSG_EXT_PPR_OPTION_DT_CRC_QUICK 0x04
#define MSG_EXT_PPR_OPTION_DT_UNITS_QUICK 0x05
