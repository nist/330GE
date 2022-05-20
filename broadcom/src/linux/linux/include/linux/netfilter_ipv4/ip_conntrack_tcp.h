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
#ifndef _IP_CONNTRACK_TCP_H
#define _IP_CONNTRACK_TCP_H
/* TCP tracking. */

enum tcp_conntrack {
	TCP_CONNTRACK_NONE,
	TCP_CONNTRACK_ESTABLISHED,
	TCP_CONNTRACK_SYN_SENT,
	TCP_CONNTRACK_SYN_RECV,
	TCP_CONNTRACK_FIN_WAIT,
	TCP_CONNTRACK_TIME_WAIT,
	TCP_CONNTRACK_CLOSE,
	TCP_CONNTRACK_CLOSE_WAIT,
	TCP_CONNTRACK_LAST_ACK,
	TCP_CONNTRACK_LISTEN,
	TCP_CONNTRACK_MAX
};

struct ip_ct_tcp
{
	enum tcp_conntrack state;

	/* Poor man's window tracking: sequence number of valid ACK
           handshake completion packet */
	u_int32_t handshake_ack;
};

#endif /* _IP_CONNTRACK_TCP_H */
