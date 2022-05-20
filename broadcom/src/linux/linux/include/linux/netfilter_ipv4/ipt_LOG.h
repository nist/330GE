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
#ifndef _IPT_LOG_H
#define _IPT_LOG_H

#define IPT_LOG_TCPSEQ		0x01	/* Log TCP sequence numbers */
#define IPT_LOG_TCPOPT		0x02	/* Log TCP options */
#define IPT_LOG_IPOPT		0x04	/* Log IP options */
#define IPT_LOG_MASK		0x07

struct ipt_log_info {
	unsigned char level;
	unsigned char logflags;
	char prefix[30];
};

#endif /*_IPT_LOG_H*/
