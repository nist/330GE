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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000 - 2001 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_SN1_HUBSTAT_H
#define _ASM_IA64_SN_SN1_HUBSTAT_H

typedef int64_t 	hub_count_t;

#define HUBSTAT_VERSION	1

typedef struct hubstat_s {
        char            hs_version;		/* structure version    */
        cnodeid_t       hs_cnode;       	/* cnode of this hub    */
        nasid_t         hs_nasid;       	/* Nasid of same        */	
	int64_t		hs_timebase;		/* Time of first sample */
	int64_t		hs_timestamp;		/* Time of last sample	*/
	int64_t		hs_per_minute;		/* Ticks per minute 	*/

	union {
		hubreg_t	hs_niu_stat_rev_id; /* SN0: Status rev ID */
		hubreg_t	hs_niu_port_status; /* SN1: Port status */
	} hs_niu;

        hub_count_t	hs_ni_retry_errors;	/* Total retry errors   */
        hub_count_t	hs_ni_sn_errors;	/* Total sn errors      */
        hub_count_t	hs_ni_cb_errors;	/* Total cb errors      */
        int		hs_ni_overflows;	/* NI count overflows   */
        hub_count_t	hs_ii_sn_errors;	/* Total sn errors      */
        hub_count_t	hs_ii_cb_errors;	/* Total cb errors      */
        int		hs_ii_overflows;	/* II count overflows   */

	/*
	 * Anything below this comment is intended for kernel internal-use
	 * only and may be changed at any time.
	 *
	 * Any members that contain pointers or are conditionally compiled
	 * need to be below here also.
	 */
        int64_t		hs_last_print;		/* When we last printed */
        char		hs_print;		/* Should we print      */

	char	       *hs_name;		/* This hub's name */
	unsigned char	hs_maint;		/* Should we print to availmon */
} hubstat_t;

#define       hs_ni_stat_rev_id       hs_niu.hs_niu_stat_rev_id
#define       hs_ni_port_status       hs_niu.hs_niu_port_status

extern struct file_operations hub_mon_fops;

#endif /* _ASM_IA64_SN_SN1_HUBSTAT_H */
