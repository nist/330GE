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
/* Definitions for Econet sockets. */

#ifndef __LINUX_IF_EC
#define __LINUX_IF_EC

/* User visible stuff. Glibc provides its own but libc5 folk will use these */

struct ec_addr
{
  unsigned char station;		/* Station number.  */
  unsigned char net;			/* Network number.  */
};

struct sockaddr_ec
{
  unsigned short sec_family;
  unsigned char port;			/* Port number.  */
  unsigned char cb;			/* Control/flag byte.  */
  unsigned char type;			/* Type of message.  */
  struct ec_addr addr;
  unsigned long cookie;
};

#define ECTYPE_PACKET_RECEIVED		0	/* Packet received */
#define ECTYPE_TRANSMIT_STATUS		0x10	/* Transmit completed, 
						   low nibble holds status */

#define ECTYPE_TRANSMIT_OK		1
#define ECTYPE_TRANSMIT_NOT_LISTENING	2
#define ECTYPE_TRANSMIT_NET_ERROR	3
#define ECTYPE_TRANSMIT_NO_CLOCK	4
#define ECTYPE_TRANSMIT_LINE_JAMMED	5
#define ECTYPE_TRANSMIT_NOT_PRESENT	6

#ifdef __KERNEL__

#define EC_HLEN				6

/* This is what an Econet frame looks like on the wire. */
struct ec_framehdr 
{
  unsigned char dst_stn;
  unsigned char dst_net;
  unsigned char src_stn;
  unsigned char src_net;
  unsigned char cb;
  unsigned char port;
};

struct econet_opt
{
  unsigned char cb;
  unsigned char port;
  unsigned char station;
  unsigned char net;
};

struct ec_device
{
  unsigned char station, net;		/* Econet protocol address */
};

#endif

#endif