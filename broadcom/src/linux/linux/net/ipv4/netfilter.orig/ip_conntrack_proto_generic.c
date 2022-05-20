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
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4/ip_conntrack_protocol.h>

#define GENERIC_TIMEOUT (600*HZ)

static int generic_pkt_to_tuple(const void *datah, size_t datalen,
				struct ip_conntrack_tuple *tuple)
{
	tuple->src.u.all = 0;
	tuple->dst.u.all = 0;

	return 1;
}

static int generic_invert_tuple(struct ip_conntrack_tuple *tuple,
				const struct ip_conntrack_tuple *orig)
{
	tuple->src.u.all = 0;
	tuple->dst.u.all = 0;

	return 1;
}

/* Print out the per-protocol part of the tuple. */
static unsigned int generic_print_tuple(char *buffer,
					const struct ip_conntrack_tuple *tuple)
{
	return 0;
}

/* Print out the private part of the conntrack. */
static unsigned int generic_print_conntrack(char *buffer,
					    const struct ip_conntrack *state)
{
	return 0;
}

/* Returns verdict for packet, or -1 for invalid. */
static int established(struct ip_conntrack *conntrack,
		       struct iphdr *iph, size_t len,
		       enum ip_conntrack_info conntrackinfo)
{
	ip_ct_refresh(conntrack, GENERIC_TIMEOUT);
	return NF_ACCEPT;
}

/* Called when a new connection for this protocol found. */
static int
new(struct ip_conntrack *conntrack, struct iphdr *iph, size_t len)
{
	return 1;
}

struct ip_conntrack_protocol ip_conntrack_generic_protocol
= { { NULL, NULL }, 0, "unknown",
    generic_pkt_to_tuple, generic_invert_tuple, generic_print_tuple,
    generic_print_conntrack, established, new, NULL, NULL, NULL };

