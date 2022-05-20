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
#ifndef _IP_NAT_RULE_H
#define _IP_NAT_RULE_H
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_nat.h>

#ifdef __KERNEL__

extern int ip_nat_rule_init(void) __init;
extern void ip_nat_rule_cleanup(void);
extern int ip_nat_rule_find(struct sk_buff **pskb,
			    unsigned int hooknum,
			    const struct net_device *in,
			    const struct net_device *out,
			    struct ip_conntrack *ct,
			    struct ip_nat_info *info);
#endif
#endif /* _IP_NAT_RULE_H */
