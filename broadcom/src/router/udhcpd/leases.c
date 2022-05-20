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
 * leases.c -- tools to manage DHCP leases 
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"
#include "arpping.h"

#include <bcmnvram.h>

unsigned char blank_chaddr[] = {[0 ... 15] = 0};
struct dhcpOfferedAddr *static_lease;
	
#ifndef BRCM_UDHCPD
#include <sys/sysinfo.h>
static long uptime(void)
{
	struct sysinfo info;
	sysinfo(&info);

	return info.uptime;
}
#endif

/* clear every lease out that chaddr OR yiaddr matches and is nonzero */
void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr)
{
	unsigned int i, j;
	
	//added by Joey to handle static lease
	static_lease = NULL;
	
	for (j = 0; j < 16 && !chaddr[j]; j++);
	
	for (i = 0; i < server_config.max_leases; i++)
	{
		// added by Joey to handle static lease
		//if (memcmp(leases[i].chaddr, chaddr, 16)==0 && leases[i].expires==0xffffffff)
		//	static_lease = &leases[i];

		if ((j != 16 && !memcmp(leases[i].chaddr, chaddr, 16)) ||
		    (yiaddr && leases[i].yiaddr == yiaddr)) 
		{	
			if (leases[i].expires==0xffffffff) static_lease=&leases[i];
			else memset(&(leases[i]), 0, sizeof(struct dhcpOfferedAddr));
		}
	}
}


/* add a lease into the table, clearing out any old ones */
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease)
{
	struct dhcpOfferedAddr *oldest;
	
	/* clean out any old ones */	
	clear_lease(chaddr, yiaddr);

	// added by Joey to handle static lease
	if (static_lease) return(static_lease);
		
	oldest = oldest_expired_lease();
	
	if (oldest) {
		memcpy(oldest->chaddr, chaddr, 16);
		oldest->yiaddr = yiaddr;
		if (lease==0xffffffff)
			oldest->expires = 0xffffffff;
		else
#ifdef BRCM_UDHCPD
			oldest->expires = time(0) + lease;
#else
			oldest->expires = uptime() + lease;
#endif
	}
	
	return oldest;
}


/* true if a lease has expired */
int lease_expired(struct dhcpOfferedAddr *lease)
{
#ifdef BRCM_UDHCPD
	return (lease->expires < (unsigned long) time(0));
#else
	return (lease->expires < (unsigned long) uptime());
#endif
}	


/* Find the oldest expired lease, NULL if there are no expired leases */
struct dhcpOfferedAddr *oldest_expired_lease(void)
{
	struct dhcpOfferedAddr *oldest = NULL;
#ifdef BRCM_UDHCPD
	unsigned long oldest_lease = time(0);
#else
	unsigned long oldest_lease = uptime();
#endif
	unsigned int i;

	
	for (i = 0; i < server_config.max_leases; i++)
		if (oldest_lease > leases[i].expires) {
			oldest_lease = leases[i].expires;
			oldest = &(leases[i]);
		}
	return oldest;
		
}


/* Find the first lease that matches chaddr, NULL if no match */
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr)
{
	unsigned int i;

	for (i = 0; i < server_config.max_leases; i++)
		//JYWeng: 20030701 for palm: chaddr error after first 6 bytes
		//if (!memcmp(leases[i].chaddr, chaddr, 16)) return &(leases[i]);
		if (!memcmp(leases[i].chaddr, chaddr, 6)) return &(leases[i]);
	
	return NULL;
}


/* Find the first lease that matches yiaddr, NULL is no match */
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr)
{
	unsigned int i;

	for (i = 0; i < server_config.max_leases; i++)
		if (leases[i].yiaddr == yiaddr) return &(leases[i]);
	
	return NULL;
}


/* find an assignable address, it check_expired is true, we check all the expired leases as well.
 * Maybe this should try expired leases by age... */
u_int32_t find_address(int check_expired) 
{
	u_int32_t addr, ret;
	struct dhcpOfferedAddr *lease = NULL;		

	addr = ntohl(server_config.start); /* addr is in host order here */
	for (;addr <= ntohl(server_config.end); addr++) {

		/* ie, 192.168.55.0 */
		if (!(addr & 0xFF)) continue;

		/* ie, 192.168.55.255 */
		if ((addr & 0xFF) == 0xFF) continue;

		/* lease is not taken */
		ret = htonl(addr);
		if ((!(lease = find_lease_by_yiaddr(ret)) ||

		     /* or it expired and we are checking for expired leases */
		     (check_expired  && lease_expired(lease))) &&

		     /* and it isn't on the network */
	    	     !check_ip(ret) &&
	    	     
	    	     /* and different from LAN IP */
	    	     check_lan_ip(ret)) {
			return ret;
			break;
		}
	}
	return 0;
}


/* check if an IP is taken, if it is, add it to the lease table */
int check_ip(u_int32_t addr)
{
	struct in_addr temp;
	
	if (arpping(addr, server_config.server, server_config.arp, server_config.interface) == 0) {
		temp.s_addr = addr;
	 	LOG(LOG_INFO, "%s belongs to someone, reserving it for %ld seconds", 
	 		inet_ntoa(temp), server_config.conflict_time);
		add_lease(blank_chaddr, addr, server_config.conflict_time);
		return 1;
	} else return 0;
}

/* check if an IP is used by LAN IP */
int check_lan_ip(u_int32_t addr)
{
	struct in_addr temp;
	temp.s_addr = addr;

	if (nvram_match("lan_ipaddr_t", inet_ntoa(temp)))
		return 0;
	else
		return 1;
}

