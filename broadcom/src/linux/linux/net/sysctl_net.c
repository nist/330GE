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
/* -*- linux-c -*-
 * sysctl_net.c: sysctl interface to net subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net directories for each protocol family. [MS]
 *
 * $Log: sysctl_net.c,v $
 * Revision 1.1.1.1  2007/01/25 12:52:10  jiahao_jhou
 *
 *
 * Revision 1.1.1.1  2003/02/03 22:38:20  mhuang
 * LINUX_2_4 branch snapshot from linux-mips.org CVS
 *
 * Revision 1.2  1996/05/08  20:24:40  shaver
 * Added bits for NET_BRIDGE and the NET_IPV4_ARP stuff and
 * NET_IPV4_IP_FORWARD.
 *
 *
 */

#include <linux/config.h>
#include <linux/mm.h>
#include <linux/sysctl.h>

#ifdef CONFIG_INET
extern ctl_table ipv4_table[];
#endif

extern ctl_table core_table[];

#ifdef CONFIG_NET
extern ctl_table ether_table[], e802_table[];
#endif

#ifdef CONFIG_IPV6
extern ctl_table ipv6_table[];
#endif

#ifdef CONFIG_TR
extern ctl_table tr_table[];
#endif

ctl_table net_table[] = {
	{NET_CORE,   "core",      NULL, 0, 0555, core_table},      
#ifdef CONFIG_NET
	{NET_802,    "802",       NULL, 0, 0555, e802_table},
	{NET_ETHER,  "ethernet",  NULL, 0, 0555, ether_table},
#endif
#ifdef CONFIG_INET
	{NET_IPV4,   "ipv4",      NULL, 0, 0555, ipv4_table},
#endif
#ifdef CONFIG_IPV6
	{NET_IPV6, "ipv6", NULL, 0, 0555, ipv6_table},
#endif
#ifdef CONFIG_TR
	{NET_TR, "token-ring", NULL, 0, 0555, tr_table},
#endif
	{0}
};
