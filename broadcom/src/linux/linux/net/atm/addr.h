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
/* net/atm/addr.h - Local ATM address registry */

/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */


#ifndef NET_ATM_ADDR_H
#define NET_ATM_ADDR_H

#include <linux/atm.h>
#include <linux/atmdev.h>


void atm_reset_addr(struct atm_dev *dev);
int atm_add_addr(struct atm_dev *dev,struct sockaddr_atmsvc *addr);
int atm_del_addr(struct atm_dev *dev,struct sockaddr_atmsvc *addr);
int atm_get_addr(struct atm_dev *dev,struct sockaddr_atmsvc *u_buf,int size);

#endif
