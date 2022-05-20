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
 * File...........: linux/drivers/s390/block/dasd_9336_erp.c
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 2000
 */

#include <asm/ccwcache.h>
#include "dasd_int.h"
#include "dasd_9336_erp.h"

#ifdef PRINTK_HEADER
#undef PRINTK_HEADER
#define PRINTK_HEADER "dasd_erp(9336)"
#endif				/* PRINTK_HEADER */

/*
 * DASD_9336_ERP_EXAMINE 
 *
 * DESCRIPTION
 *   Checks only for fatal/no/recover error. 
 *   A detailed examination of the sense data is done later outside
 *   the interrupt handler.
 *
 *   The logic is based on the 'IBM 3880 Storage Control Reference' manual
 *   'Chapter 7. 9336 Sense Data'.
 *
 * RETURN VALUES
 *   dasd_era_none      no error 
 *   dasd_era_fatal     for all fatal (unrecoverable errors)
 *   dasd_era_recover   for all others.
 */
dasd_era_t dasd_9336_erp_examine (ccw_req_t * cqr, devstat_t * stat)
{
	/* check for successful execution first */
	if (stat->cstat == 0x00 &&
	    stat->dstat == (DEV_STAT_CHN_END | DEV_STAT_DEV_END))
		    return dasd_era_none;

	/* examine the 24 byte sense data */
	return dasd_era_recover;

}				/* END dasd_9336_erp_examine */

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-indent-level: 4 
 * c-brace-imaginary-offset: 0
 * c-brace-offset: -4
 * c-argdecl-indent: 4
 * c-label-offset: -4
 * c-continued-statement-offset: 4
 * c-continued-brace-offset: 0
 * indent-tabs-mode: nil
 * tab-width: 8
 * End:
 */
