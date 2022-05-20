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
/***   ltpc.h
 *
 *
 ***/

#define LT_GETRESULT  0x00
#define LT_WRITEMEM   0x01
#define LT_READMEM    0x02
#define LT_GETFLAGS   0x04
#define LT_SETFLAGS   0x05
#define LT_INIT       0x10
#define LT_SENDLAP    0x13
#define LT_RCVLAP     0x14

/* the flag that we care about */
#define LT_FLAG_ALLLAP 0x04

struct lt_getresult {
	unsigned char command;
	unsigned char mailbox;
};

struct lt_mem {
	unsigned char command;
	unsigned char mailbox;
	unsigned short addr;	/* host order */
	unsigned short length;	/* host order */
};

struct lt_setflags {
	unsigned char command;
	unsigned char mailbox;
	unsigned char flags;
};

struct lt_getflags {
	unsigned char command;
	unsigned char mailbox;
};

struct lt_init {
	unsigned char command;
	unsigned char mailbox;
	unsigned char hint;
};

struct lt_sendlap {
	unsigned char command;
	unsigned char mailbox;
	unsigned char dnode;
	unsigned char laptype;
	unsigned short length;	/* host order */
};

struct lt_rcvlap {
	unsigned char command;
	unsigned char dnode;
	unsigned char snode;
	unsigned char laptype;
	unsigned short length;	/* host order */
};

union lt_command {
	struct lt_getresult getresult;
	struct lt_mem mem;
	struct lt_setflags setflags;
	struct lt_getflags getflags;
	struct lt_init init;
	struct lt_sendlap sendlap;
	struct lt_rcvlap rcvlap;
};
typedef union lt_command lt_command;

