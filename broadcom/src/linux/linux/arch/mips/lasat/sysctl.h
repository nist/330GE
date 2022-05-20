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
 * LASAT sysctl values
 */

#ifndef _LASAT_SYSCTL_H
#define _LASAT_SYSCTL_H

/* /proc/sys/lasat */
enum {
	LASAT_CPU_HZ=1,
	LASAT_BUS_HZ=2,
	LASAT_MODEL=3,
	LASAT_SERIAL=4,
	LASAT_PARTNO=5,
	LASAT_EDHAC=6,
	LASAT_EADI=7,
	LASAT_LEASEDLINE=8,
	LASAT_ISDN=9,
	LASAT_HIFN=10,
	LASAT_USVER=11,
	LASAT_PRID=12,
	LASAT_IPADDR=13,
	LASAT_NETMASK=14,
	LASAT_PASSWORD=15,
	LASAT_SERVICEFLAG=16,
	LASAT_VENDID=17,
	LASAT_SBOOT=18,
	LASAT_RTC=19,
/*	LASAT_CFG=20,		*/
	LASAT_VER=21,
/*	LASAT_WANLED=22,	*/
	LASAT_UPGRADE=23,
	LASAT_VPN_KBPS=24,
	LASAT_TUNNELS=25,
	LASAT_CLIENTS=26,
	LASAT_VENDSTR=27,
	LASAT_NAMESTR=28,
	LASAT_TYPESTR=29,
	LASAT_SERVICEVER=30,
	LASAT_DEBUGACCESS=31,
};

#endif /* _LASAT_SYSCTL_H */
