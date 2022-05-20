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
/*      cops.h: LocalTalk driver for Linux.
 *
 *      Authors:
 *      - Jay Schulist <jschlst@samba.org>
 */

#ifndef __LINUX_COPSLTALK_H
#define __LINUX_COPSLTALK_H

#ifdef __KERNEL__

/* Max LLAP size we will accept. */
#define MAX_LLAP_SIZE		603

/* Tangent */
#define TANG_CARD_STATUS        1
#define TANG_CLEAR_INT          1
#define TANG_RESET              3

#define TANG_TX_READY           1
#define TANG_RX_READY           2

/* Dayna */
#define DAYNA_CMD_DATA          0
#define DAYNA_CLEAR_INT         1
#define DAYNA_CARD_STATUS       2
#define DAYNA_INT_CARD          3
#define DAYNA_RESET             4

#define DAYNA_RX_READY          0
#define DAYNA_TX_READY          1
#define DAYNA_RX_REQUEST        3

/* Same on both card types */
#define COPS_CLEAR_INT  1

/* LAP response codes received from the cards. */
#define LAP_INIT        1       /* Init cmd */
#define LAP_INIT_RSP    2       /* Init response */
#define LAP_WRITE       3       /* Write cmd */
#define DATA_READ       4       /* Data read */
#define LAP_RESPONSE    4       /* Received ALAP frame response */
#define LAP_GETSTAT     5       /* Get LAP and HW status */
#define LAP_RSPSTAT     6       /* Status response */

#endif

/*
 *	Structure to hold the firmware information.
 */
struct ltfirmware
{
        unsigned int length;
        unsigned char * data;
};

#define DAYNA 1
#define TANGENT 2

#endif
