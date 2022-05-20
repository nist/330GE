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
 * ioctl defines for ethernet driver
 *
 * Copyright (c) 2001 Axis Communications AB
 * 
 * Author: Mikael Starvik 
 *
 */

#ifndef _CRIS_ETHERNET_H
#define _CRIS_ETHERNET_H
#define SET_ETH_SPEED_AUTO      SIOCDEVPRIVATE          /* Auto neg speed */
#define SET_ETH_SPEED_10        SIOCDEVPRIVATE+1        /* 10 Mbps */
#define SET_ETH_SPEED_100       SIOCDEVPRIVATE+2        /* 100 Mbps. */
#define SET_ETH_DUPLEX_AUTO     SIOCDEVPRIVATE+3        /* Auto neg duplex */
#define SET_ETH_DUPLEX_HALF     SIOCDEVPRIVATE+4        /* Full duplex */
#define SET_ETH_DUPLEX_FULL     SIOCDEVPRIVATE+5        /* Half duplex */
#endif /* _CRIS_ETHERNET_H */
