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
/***************************************************************************
 *
 *  drivers/s390/char/tape3490.c
 *    tape device discipline for 3490E tapes.
 *
 *  S390 and zSeries version
 *    Copyright (C) 2001 IBM Corporation
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 *               Tuan Ngo-Anh <ngoanh@de.ibm.com>
 *
 ****************************************************************************
 */

#include "tapedefs.h"
#include <linux/version.h>
#include <asm/ccwcache.h>	/* CCW allocations      */
#include <asm/s390dyn.h>
#include <asm/debug.h>
#include <linux/compatmac.h>
#include "tape.h"
#include "tape34xx.h"
#include "tape3490.h"

tape_event_handler_t tape3490_event_handler_table[TS_SIZE][TE_SIZE] =
{
    /* {START , DONE, FAILED, ERROR, OTHER } */
	{NULL, tape34xx_unused_done, NULL, NULL, NULL},	/* TS_UNUSED */
	{NULL, tape34xx_idle_done, NULL, NULL, NULL},	/* TS_IDLE */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_DONE */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_FAILED */
	{NULL, tape34xx_block_done, NULL, NULL, NULL},		/* TS_BLOCK_INIT */
	{NULL, tape34xx_bsb_init_done, NULL, NULL, NULL},	/* TS_BSB_INIT */
	{NULL, tape34xx_bsf_init_done, NULL, NULL, NULL},	/* TS_BSF_INIT */
	{NULL, tape34xx_dse_init_done, NULL, NULL, NULL},	/* TS_DSE_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_EGA_INIT */
	{NULL, tape34xx_fsb_init_done, NULL, NULL, NULL},	/* TS_FSB_INIT */
	{NULL, tape34xx_fsf_init_done, NULL, NULL, NULL},	/* TS_FSF_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_LDI_INIT */
	{NULL, tape34xx_lbl_init_done, NULL, NULL, NULL},	/* TS_LBL_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_MSE_INIT */
	{NULL, tape34xx_nop_init_done, NULL, NULL, NULL},	/* TS_NOP_INIT */
	{NULL, tape34xx_rfo_init_done, NULL, NULL, NULL},		/* TS_RBA_INIT */
	{NULL, tape34xx_rbi_init_done, NULL, NULL, NULL},	/* TS_RBI_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_RBU_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_RBL_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_RDC_INIT */
	{NULL, tape34xx_rfo_init_done, NULL, NULL, NULL},	/* TS_RFO_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_RSD_INIT */
	{NULL, tape34xx_rew_init_done, NULL, NULL, NULL},	/* TS_REW_INIT */
	{NULL, tape34xx_rew_release_init_done, NULL, NULL, NULL},	/* TS_REW_RELEASE_IMIT */
	{NULL, tape34xx_run_init_done, NULL, NULL, NULL},	/* TS_RUN_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SEN_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SID_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SNP_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SPG_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SWI_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SMR_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_SYN_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_TIO_INIT */
	{NULL, NULL, NULL, NULL, NULL},		/* TS_UNA_INIT */
	{NULL, tape34xx_wri_init_done, NULL, NULL, NULL},	/* TS_WRI_INIT */
	{NULL, tape34xx_wtm_init_done, NULL, NULL, NULL},	/* TS_WTM_INIT */
        {NULL, NULL, NULL, NULL, NULL}};     /* TS_NOT_OPER */

devreg_t tape3490_devreg = {
    ci:
    {hc:
     {ctype:0x3490}},
    flag:DEVREG_MATCH_CU_TYPE | DEVREG_TYPE_DEVCHARS,
    oper_func:tape_oper_handler
};

void
tape3490_setup_assist (tape_info_t * ti)
{
	tape3490_disc_data_t *data = NULL;
#ifdef TAPE_DEBUG
    debug_text_event (tape_debug_area,6,"3490 dsetu");
    debug_text_event (tape_debug_area,6,"dev:");
    debug_int_event (tape_debug_area,6,ti->blk_minor);
#endif /* TAPE_DEBUG */
	while (data == NULL)
		data = kmalloc (sizeof (tape3490_disc_data_t), GFP_KERNEL);
	data->modeset_byte = 0x00;
	ti->discdata = (void *) data;
}


void
tape3490_shutdown (int autoprobe) {
    if (autoprobe)
	s390_device_unregister(&tape3490_devreg);
}


tape_discipline_t *
tape3490_init (int autoprobe)
{
	tape_discipline_t *disc;
#ifdef TAPE_DEBUG
    debug_text_event (tape_debug_area,3,"3490 init");
#endif /* TAPE_DEBUG */
	disc = kmalloc (sizeof (tape_discipline_t), GFP_KERNEL);
	if (disc == NULL) {
#ifdef TAPE_DEBUG
	        debug_text_exception (tape_debug_area,3,"disc:nomem");
#endif /* TAPE_DEBUG */
		return disc;
	}
	disc->cu_type = 0x3490;
	disc->setup_assist = tape3490_setup_assist;
	disc->error_recovery = tape34xx_error_recovery;
	disc->write_block = tape34xx_write_block;
	disc->free_write_block = tape34xx_free_write_block;
	disc->read_block = tape34xx_read_block;
	disc->free_read_block = tape34xx_free_read_block;
	disc->mtfsf = tape34xx_mtfsf;
	disc->mtbsf = tape34xx_mtbsf;
	disc->mtfsr = tape34xx_mtfsr;
	disc->mtbsr = tape34xx_mtbsr;
	disc->mtweof = tape34xx_mtweof;
	disc->mtrew = tape34xx_mtrew;
	disc->mtoffl = tape34xx_mtoffl;
	disc->mtnop = tape34xx_mtnop;
	disc->mtbsfm = tape34xx_mtbsfm;
	disc->mtfsfm = tape34xx_mtfsfm;
	disc->mteom = tape34xx_mteom;
	disc->mterase = tape34xx_mterase;
	disc->mtsetdensity = tape34xx_mtsetdensity;
	disc->mtseek = tape34xx_mtseek;
	disc->mttell = tape34xx_mttell;
	disc->mtsetdrvbuffer = tape34xx_mtsetdrvbuffer;
	disc->mtlock = tape34xx_mtlock;
	disc->mtunlock = tape34xx_mtunlock;
	disc->mtload = tape34xx_mtload;
	disc->mtunload = tape34xx_mtunload;
	disc->mtcompression = tape34xx_mtcompression;
	disc->mtsetpart = tape34xx_mtsetpart;
	disc->mtmkpart = tape34xx_mtmkpart;
	disc->mtiocget = tape34xx_mtiocget;
	disc->mtiocpos = tape34xx_mtiocpos;
	disc->shutdown = tape3490_shutdown;
	disc->discipline_ioctl_overload = tape34xx_ioctl_overload;
	disc->event_table = &tape3490_event_handler_table;
	disc->default_handler = tape34xx_default_handler;
	disc->bread = tape34xx_bread;
	disc->free_bread = tape34xx_free_bread;
	disc->tape = NULL;	/* pointer for backreference */
	disc->next = NULL;
	if (autoprobe)
	    s390_device_register(&tape3490_devreg);
#ifdef TAPE_DEBUG
	debug_text_event (tape_debug_area,3,"3490 regis");
#endif /* TAPE_DEBUG */
	return disc;
}
