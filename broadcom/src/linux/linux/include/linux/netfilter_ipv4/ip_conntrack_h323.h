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
#ifndef _IP_CONNTRACK_H323_H
#define _IP_CONNTRACK_H323_H
/* H.323 connection tracking. */

#ifdef __KERNEL__
/* Protects H.323 related data */
DECLARE_LOCK_EXTERN(ip_h323_lock);
#endif

/* Default H.225 port */
#define H225_PORT	1720

/* This structure is per expected connection */
struct ip_ct_h225_expect {
	u_int16_t port;			/* Port of the H.225 helper/RTCP/RTP channel */
	enum ip_conntrack_dir dir;	/* Direction of the original connection */
	unsigned int offset;		/* offset of the address in the payload */
};

/* This structure exists only once per master */
struct ip_ct_h225_master {
	int is_h225;				/* H.225 or H.245 connection */
#ifdef CONFIG_IP_NF_NAT_NEEDED
	enum ip_conntrack_dir dir;		/* Direction of the original connection */
	u_int32_t seq[IP_CT_DIR_MAX];		/* Exceptional packet mangling for signal addressess... */
	unsigned int offset[IP_CT_DIR_MAX];	/* ...and the offset of the addresses in the payload */
#endif
};

#endif /* _IP_CONNTRACK_H323_H */
