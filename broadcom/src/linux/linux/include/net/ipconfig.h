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
/*
 *  $Id: ipconfig.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 *
 *  Copyright (C) 1997 Martin Mares
 *
 *  Automatic IP Layer Configuration
 */

/* The following are initdata: */

extern int ic_enable;		/* Enable or disable the whole shebang */

extern int ic_proto_enabled;	/* Protocols enabled (see IC_xxx) */
extern int ic_host_name_set;	/* Host name set by ipconfig? */
extern int ic_set_manually;	/* IPconfig parameters set manually */

extern u32 ic_myaddr;		/* My IP address */
extern u32 ic_netmask;		/* Netmask for local subnet */
extern u32 ic_gateway;		/* Gateway IP address */

extern u32 ic_servaddr;		/* Boot server IP address */

extern u32 root_server_addr;	/* Address of NFS server */
extern u8 root_server_path[];	/* Path to mount as root */



/* The following are persistent (not initdata): */

extern int ic_proto_used;	/* Protocol used, if any */
extern u32 ic_nameserver;	/* DNS server IP address */
extern u8 ic_domain[];		/* DNS (not NIS) domain name */

/* bits in ic_proto_{enabled,used} */
#define IC_PROTO	0xFF	/* Protocols mask: */
#define IC_BOOTP	0x01	/*   BOOTP (or DHCP, see below) */
#define IC_RARP		0x02	/*   RARP */
#define IC_USE_DHCP    0x100	/* If on, use DHCP instead of BOOTP */
