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
 * File...........: linux/drivers/s390/block/dasd_3990_erp.h
 * Author(s)......: Horst Hummel <Horst Hummel@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 2000
 *
 * History of changes (starts July 2000)
 */

#ifndef DASD_3990_ERP_H
#define DASD_3990_ERP_H

dasd_era_t dasd_3990_erp_examine (ccw_req_t *, devstat_t *);

ccw_req_t *dasd_3990_erp_action (ccw_req_t *);
ccw_req_t *dasd_2105_erp_action (ccw_req_t *);

void dasd_3990_erp_restart_queue (unsigned long);

typedef struct DCTL_data_t {
	unsigned char subcommand;	/* e.g Inhibit Write, Enable Write,... */
	unsigned char modifier;	/* Subcommand modifier                 */
	unsigned short res;	/* reserved */
} __attribute__ ((packed)) DCTL_data_t;

#endif				/* DASD_3990_ERP_H */
