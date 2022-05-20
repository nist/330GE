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
#ifndef __ipt_time_h_included__
#define __ipt_time_h_included__


struct ipt_time_info {
	unsigned int days_match;	/* 1 bit per day (bit 0 = Sunday) */
	unsigned int time_start;	/* 0 < time_start < 24*60*60-1 = 86399 */
	unsigned int time_stop;		/* 0 < time_end < 24*60*60-1 = 86399 */
	int kerneltime;			/* ignore skb time (and use kerneltime) or not. */
};


#endif /* __ipt_time_h_included__ */
