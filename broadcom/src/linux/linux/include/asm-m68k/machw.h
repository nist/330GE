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
** linux/machw.h -- This header defines some macros and pointers for
**                    the various Macintosh custom hardware registers.
**
** Copyright 1997 by Michael Schmitz
**
** This file is subject to the terms and conditions of the GNU General Public
** License.  See the file COPYING in the main directory of this archive
** for more details.
**
*/

#ifndef _ASM_MACHW_H_
#define _ASM_MACHW_H_

/*
 * head.S maps the videomem to VIDEOMEMBASE
 */

#define VIDEOMEMBASE	0xf0000000
#define VIDEOMEMSIZE	(4096*1024)
#define VIDEOMEMMASK	(-4096*1024)

#ifndef __ASSEMBLY__

#include <linux/types.h>


/* hardware stuff */

#define MACHW_DECLARE(name)	unsigned name : 1
#define MACHW_SET(name)		(mac_hw_present.name = 1)
#define MACHW_PRESENT(name)	(mac_hw_present.name)

struct mac_hw_present {
  /* video hardware */
  /* sound hardware */
  /* disk storage interfaces */
  MACHW_DECLARE(MAC_SCSI_80);     /* Directly mapped NCR5380 */
  MACHW_DECLARE(MAC_SCSI_96);     /* 53c9[46] */
  MACHW_DECLARE(MAC_SCSI_96_2);   /* 2nd 53c9[46] Q900 and Q950 */
  MACHW_DECLARE(IDE);             /* IDE Interface */
  /* other I/O hardware */
  MACHW_DECLARE(SCC);             /* Serial Communications Contr. */
  /* DMA */
  MACHW_DECLARE(SCSI_DMA);        /* DMA for the NCR5380 */
  /* real time clocks */
  MACHW_DECLARE(RTC_CLK);         /* clock chip */
  /* supporting hardware */
  MACHW_DECLARE(VIA1);            /* Versatile Interface Ad. 1 */
  MACHW_DECLARE(VIA2);            /* Versatile Interface Ad. 2 */
  MACHW_DECLARE(RBV);             /* Versatile Interface Ad. 2+ */
  /* NUBUS */
  MACHW_DECLARE(NUBUS);           /* NUBUS */
};

extern struct mac_hw_present mac_hw_present;

#endif /* __ASSEMBLY__ */

#endif /* linux/machw.h */
