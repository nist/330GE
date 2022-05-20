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
 *  arch/s390/kernel/s390dyn.h
 *   S/390 data definitions for dynamic device attachment
 *
 *  S390 version
 *    Copyright (C) 2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Ingo Adlung (adlung@de.ibm.com)
 */

#ifndef __s390dyn_h
#define __s390dyn_h

#ifndef _LINUX_LIST_H
#include <linux/list.h>
#endif

struct _devreg;

typedef  int  (* oper_handler_func_t)( int             irq,
                                       struct _devreg *dreg);

typedef struct _devreg_hc_t {
	__u16 ctype;
	__u8  cmode;
	__u16 dtype;
	__u8  dmode;
   }  __attribute__ ((packed)) devreg_hc_t;

typedef struct _devreg {
	struct list_head list;
	union {
		int devno;
		devreg_hc_t hc;       /* has controller info */
	} ci;

	int                  flag;
	oper_handler_func_t  oper_func;
} devreg_t;

#define DEVREG_MATCH_CU_TYPE    0x00000001
#define DEVREG_MATCH_CU_MODEL   0x00000002
#define DEVREG_MATCH_DEV_TYPE   0x00000004
#define DEVREG_MATCH_DEV_MODEL  0x00000008

#define DEVREG_EXACT_MATCH      (DEVREG_MATCH_CU_TYPE|DEVREG_MATCH_CU_MODEL|DEVREG_MATCH_DEV_TYPE|DEVREG_MATCH_DEV_MODEL)
#define DEVREG_NO_CU_INFO       (DEVREG_MATCH_DEV_TYPE|DEVREG_MATCH_DEV_MODEL)
#define DEVREG_NO_DEV_INFO      (DEVREG_MATCH_CU_TYPE|DEVREG_MATCH_CU_MODEL)

#define DEVREG_TYPE_DEVNO       0x80000000
#define DEVREG_TYPE_DEVCHARS    0x40000000

int        s390_device_register  ( devreg_t *drinfo );
int        s390_device_unregister( devreg_t *dreg );
devreg_t * s390_search_devreg    ( ioinfo_t *ioinfo );

#endif /* __s390dyn */
