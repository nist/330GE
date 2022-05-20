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
#ifndef _IPT_SAME_H
#define _IPT_SAME_H

#define IPT_SAME_MAX_RANGE	10

#define IPT_SAME_NODST		0x01

struct ipt_same_info
{
	unsigned char info;

	unsigned int rangesize;

	unsigned int ipnum;

	u_int32_t *iparray;

	/* hangs off end. */
	struct ip_nat_range range[IPT_SAME_MAX_RANGE];
};

#endif /*_IPT_SAME_H*/