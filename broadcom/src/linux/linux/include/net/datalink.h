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
#ifndef _NET_INET_DATALINK_H_
#define _NET_INET_DATALINK_H_

struct datalink_proto {
	unsigned short	type_len;
	unsigned char	type[8];
	const char	*string_name;
	unsigned short	header_length;
	int	(*rcvfunc)(struct sk_buff *, struct net_device *, 
				struct packet_type *);
	void	(*datalink_header)(struct datalink_proto *, struct sk_buff *,
					unsigned char *);
	struct datalink_proto	*next;
};

#endif
