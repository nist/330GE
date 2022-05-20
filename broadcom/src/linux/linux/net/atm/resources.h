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
/* net/atm/resources.h - ATM-related resources */

/* Written 1995-1998 by Werner Almesberger, EPFL LRC/ICA */


#ifndef NET_ATM_RESOURCES_H
#define NET_ATM_RESOURCES_H

#include <linux/config.h>
#include <linux/atmdev.h>


extern struct atm_dev *atm_devs;
extern struct atm_vcc *nodev_vccs; /* VCCs not linked to any device */


struct sock *alloc_atm_vcc_sk(int family);
void free_atm_vcc_sk(struct sock *sk);


#ifdef CONFIG_PROC_FS

#include <linux/proc_fs.h>

int atm_proc_dev_register(struct atm_dev *dev);
void atm_proc_dev_deregister(struct atm_dev *dev);

#endif

#endif
