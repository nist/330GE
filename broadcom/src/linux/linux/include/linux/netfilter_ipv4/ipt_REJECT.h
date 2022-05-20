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
#ifndef _IPT_REJECT_H
#define _IPT_REJECT_H

enum ipt_reject_with {
	IPT_ICMP_NET_UNREACHABLE,
	IPT_ICMP_HOST_UNREACHABLE,
	IPT_ICMP_PROT_UNREACHABLE,
	IPT_ICMP_PORT_UNREACHABLE,
	IPT_ICMP_ECHOREPLY,
	IPT_ICMP_NET_PROHIBITED,
	IPT_ICMP_HOST_PROHIBITED,
	IPT_TCP_RESET
};

struct ipt_reject_info {
	enum ipt_reject_with with;      /* reject type */
};

#endif /*_IPT_REJECT_H*/
