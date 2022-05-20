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
 * linux/dp83840.h: definitions for DP83840 MII-compatible transceivers
 *
 * Copyright (C) 1996, 1999 David S. Miller (davem@redhat.com)
 */
#ifndef __LINUX_DP83840_H
#define __LINUX_DP83840_H

#include <linux/mii.h>

/*
 * Data sheets and programming docs for the DP83840 are available at
 * from http://www.national.com/
 *
 * The DP83840 is capable of both 10 and 100Mbps ethernet, in both
 * half and full duplex mode.  It also supports auto negotiation.
 *
 * But.... THIS THING IS A PAIN IN THE ASS TO PROGRAM!
 * Debugging eeprom burnt code is more fun than programming this chip!
 */

/* First, the MII register numbers (actually DP83840 register numbers). */
#define MII_CSCONFIG        0x17        /* CS configuration            */

/* The Carrier Sense config register. */
#define CSCONFIG_RESV1          0x0001  /* Unused...                   */
#define CSCONFIG_LED4           0x0002  /* Pin for full-dplx LED4      */
#define CSCONFIG_LED1           0x0004  /* Pin for conn-status LED1    */
#define CSCONFIG_RESV2          0x0008  /* Unused...                   */
#define CSCONFIG_TCVDISAB       0x0010  /* Turns off the transceiver   */
#define CSCONFIG_DFBYPASS       0x0020  /* Bypass disconnect function  */
#define CSCONFIG_GLFORCE        0x0040  /* Good link force for 100mbps */
#define CSCONFIG_CLKTRISTATE    0x0080  /* Tristate 25m clock          */
#define CSCONFIG_RESV3          0x0700  /* Unused...                   */
#define CSCONFIG_ENCODE         0x0800  /* 1=MLT-3, 0=binary           */
#define CSCONFIG_RENABLE        0x1000  /* Repeater mode enable        */
#define CSCONFIG_TCDISABLE      0x2000  /* Disable timeout counter     */
#define CSCONFIG_RESV4          0x4000  /* Unused...                   */
#define CSCONFIG_NDISABLE       0x8000  /* Disable NRZI                */

#endif /* __LINUX_DP83840_H */
