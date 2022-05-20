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
/* Shared library add-on to iptables to add IP address pool matching. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include <iptables.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ipt_pool.h>

#include <libippool/ip_pool_support.h>

#include "../ippool/libippool.c"

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"pool v%s options:\n"
" [!] --srcpool NAME|INDEX\n"
" [!] --dstpool NAME|INDEX\n"
"			Pool index (or name from %s) to match\n"
"\n", IPTABLES_VERSION, IPPOOL_CONF);
}

static struct option opts[] = {
	{ "srcpool", 1, 0, '1' },
	{ "dstpool", 1, 0, '2' },
	{0}
};

/* Initialize the match. */
static void
init(struct ipt_entry_match *match, unsigned int *nfcache)
{
	struct ipt_pool_info *info =
		(struct ipt_pool_info *)match->data;

	info->src = IP_POOL_NONE;
	info->dst = IP_POOL_NONE;
	info->flags = 0;
	*nfcache |= NFC_UNKNOWN;
}

/* Function which parses command options; returns true if it ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      unsigned int *nfcache,
      struct ipt_entry_match **match)
{
	struct ipt_pool_info *info =
		(struct ipt_pool_info *)(*match)->data;

	switch (c) {
	case '1':
		check_inverse(optarg, &invert, &optind, 0);
		info->src = ip_pool_get_index(argv[optind-1]);
		if (invert) info->flags |= IPT_POOL_INV_SRC;
		*flags = 1;
		break;
	case '2':
		check_inverse(optarg, &invert, &optind, 0);
		info->dst = ip_pool_get_index(argv[optind-1]);
		if (invert) info->flags |= IPT_POOL_INV_DST;
		*flags = 1;
		break;

	default:
		return 0;
	}

	return 1;
}

/* Final check; must have specified --srcpool or --dstpool. */
static void final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM, "You must specify either `--srcpool or --dstpool'");
}

/* Prints out the matchinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_match *match,
      int numeric)
{
	char buf[256];
	struct ipt_pool_info *info =
		(struct ipt_pool_info *)match->data;

	if (info->src != IP_POOL_NONE)
		printf("%ssrcpool %s ",
			(info->flags & IPT_POOL_INV_SRC) ? "!" : "",
			ip_pool_get_name(buf, sizeof(buf), info->src, 0));
	if (info->dst != IP_POOL_NONE)
		printf("%sdstpool %s ",
			(info->flags & IPT_POOL_INV_DST) ? "!" : "",
			ip_pool_get_name(buf, sizeof(buf), info->dst, 0));
}

/* Saves the matchinfo in parsable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	char buf[256];
	struct ipt_pool_info *info =
		(struct ipt_pool_info *)match->data;

	if (info->src != IP_POOL_NONE)
		printf("%s--srcpool %s",
			(info->flags & IPT_POOL_INV_SRC) ? "! " : "",
			ip_pool_get_name(buf, sizeof(buf), info->src, 0));
	if (info->dst != IP_POOL_NONE)
		printf("%s--dstpool %s",
			(info->flags & IPT_POOL_INV_DST) ? "! " : "",
			ip_pool_get_name(buf, sizeof(buf), info->dst, 0));
}

static
struct iptables_match pool
= { NULL,
    "pool",
    IPTABLES_VERSION,
    IPT_ALIGN(sizeof(struct ipt_pool_info)),
    IPT_ALIGN(sizeof(struct ipt_pool_info)),
    &help,
    &init,
    &parse,
    &final_check,
    &print,
    &save,
    opts
};

void _init(void)
{
	register_match(&pool);
}
