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
/* support function prototypes for IP pool management (config file, mostly) */
#ifndef _IP_POOL_SUPPORT_H
#define _IP_POOL_SUPPORT_H

#include <iptables.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_pool.h>

#ifndef IPPOOL_CONF
#define IPPOOL_CONF "/etc/ippool.conf"
#endif

/* called just to draw in this support .o */
void ip_pool_init(void);

/* given a pool name (or number), return pool index, possibly reading .conf */
ip_pool_t ip_pool_get_index(char *name);

/* given a pool index, and a buffer to store a name, search for the index
 * in the .conf file, and give the textual name, if present; if not, the
 * numeric index is returned. If numeric_flag == 1, the numeric index is
 * always returned
 */
char *ip_pool_get_name(char *buf, int size, ip_pool_t index, int numeric_flag);

#endif /*_IP_POOL_SUPPORT_H*/
