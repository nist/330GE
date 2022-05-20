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
 * sysctl_net_atalk.c: sysctl interface to net AppleTalk subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/atalk directory entry (empty =) ). [MS]
 * Dynamic registration, added aarp entries. (5/30/97 Chris Horn)
 */

#include <linux/config.h>
#include <linux/mm.h>
#include <linux/sysctl.h>

extern int sysctl_aarp_expiry_time;
extern int sysctl_aarp_tick_time;
extern int sysctl_aarp_retransmit_limit;
extern int sysctl_aarp_resolve_time;

#ifdef CONFIG_SYSCTL
static ctl_table atalk_table[] = {
	{NET_ATALK_AARP_EXPIRY_TIME, "aarp-expiry-time",
	 &sysctl_aarp_expiry_time, sizeof(int), 0644, NULL, &proc_dointvec_jiffies},
	{NET_ATALK_AARP_TICK_TIME, "aarp-tick-time",
	 &sysctl_aarp_tick_time, sizeof(int), 0644, NULL, &proc_dointvec_jiffies},
	{NET_ATALK_AARP_RETRANSMIT_LIMIT, "aarp-retransmit-limit",
	 &sysctl_aarp_retransmit_limit, sizeof(int), 0644, NULL, &proc_dointvec},
	{NET_ATALK_AARP_RESOLVE_TIME, "aarp-resolve-time",
	 &sysctl_aarp_resolve_time, sizeof(int), 0644, NULL, &proc_dointvec_jiffies},
	{0}
};

static ctl_table atalk_dir_table[] = {
	{NET_ATALK, "appletalk", NULL, 0, 0555, atalk_table},
	{0}
};

static ctl_table atalk_root_table[] = {
	{CTL_NET, "net", NULL, 0, 0555, atalk_dir_table},
	{0}
};

static struct ctl_table_header *atalk_table_header;

void atalk_register_sysctl(void)
{
	atalk_table_header = register_sysctl_table(atalk_root_table, 1);
}

void atalk_unregister_sysctl(void)
{
	unregister_sysctl_table(atalk_table_header);
}

#else
void atalk_register_sysctl(void)
{
}

void atalk_unregister_sysctl(void)
{
}
#endif
