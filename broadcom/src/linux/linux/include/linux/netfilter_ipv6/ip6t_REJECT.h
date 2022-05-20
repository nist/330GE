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
#ifndef _IP6T_REJECT_H
#define _IP6T_REJECT_H

enum ip6t_reject_with {
	IP6T_ICMP_NET_UNREACHABLE,
	IP6T_ICMP_HOST_UNREACHABLE,
	IP6T_ICMP_PROT_UNREACHABLE,
	IP6T_ICMP_PORT_UNREACHABLE,
	IP6T_ICMP_ECHOREPLY
};

struct ip6t_reject_info {
	enum ip6t_reject_with with;      /* reject type */
};

#endif /*_IPT_REJECT_H*/
