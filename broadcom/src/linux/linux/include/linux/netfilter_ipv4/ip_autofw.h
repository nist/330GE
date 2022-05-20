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
#ifndef _IP_AUTOFW_H
#define _IP_AUTOFW_H

#define AUTOFW_MASTER_TIMEOUT 600	/* 600 secs */

struct ip_autofw_info {
	u_int16_t proto;	/* Related protocol */
	u_int16_t dport[2];	/* Related destination port range */
	u_int16_t to[2];	/* Port range to map related destination port range to */
};

struct ip_autofw_expect {
	u_int16_t dport[2];	/* Related destination port range */
	u_int16_t to[2];	/* Port range to map related destination port range to */
};

#endif /*_IP_AUTOFW_H*/
