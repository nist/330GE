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
/* $Id: memreg.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC_MEMREG_H
#define _SPARC_MEMREG_H
/* memreg.h:  Definitions of the values found in the synchronous
 *            and asynchronous memory error registers when a fault
 *            occurs on the sun4c.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

/* First the synchronous error codes, these are usually just
 * normal page faults.
 */

#define SUN4C_SYNC_WDRESET   0x0001  /* watchdog reset */
#define SUN4C_SYNC_SIZE      0x0002  /* bad access size? whuz this? */
#define SUN4C_SYNC_PARITY    0x0008  /* bad ram chips caused a parity error */
#define SUN4C_SYNC_SBUS      0x0010  /* the SBUS had some problems... */
#define SUN4C_SYNC_NOMEM     0x0020  /* translation to non-existent ram */
#define SUN4C_SYNC_PROT      0x0040  /* access violated pte protections */
#define SUN4C_SYNC_NPRESENT  0x0080  /* pte said that page was not present */
#define SUN4C_SYNC_BADWRITE  0x8000  /* while writing something went bogus */

#define SUN4C_SYNC_BOLIXED  \
        (SUN4C_SYNC_WDRESET | SUN4C_SYNC_SIZE | SUN4C_SYNC_SBUS | \
         SUN4C_SYNC_NOMEM | SUN4C_SYNC_PARITY)

/* Now the asynchronous error codes, these are almost always produced
 * by the cache writing things back to memory and getting a bad translation.
 * Bad DVMA transactions can cause these faults too.
 */

#define SUN4C_ASYNC_BADDVMA 0x0010  /* error during DVMA access */
#define SUN4C_ASYNC_NOMEM   0x0020  /* write back pointed to bad phys addr */
#define SUN4C_ASYNC_BADWB   0x0080  /* write back points to non-present page */

/* Memory parity error register with associated bit constants. */
#ifndef __ASSEMBLY__
extern __volatile__ unsigned long *sun4c_memerr_reg;
#endif

#define	SUN4C_MPE_ERROR	0x80	/* Parity error detected. (ro) */
#define	SUN4C_MPE_MULTI	0x40	/* Multiple parity errors detected. (ro) */
#define	SUN4C_MPE_TEST	0x20	/* Write inverse parity. (rw) */
#define	SUN4C_MPE_CHECK	0x10	/* Enable parity checking. (rw) */
#define	SUN4C_MPE_ERR00	0x08	/* Parity error in bits 0-7. (ro) */
#define	SUN4C_MPE_ERR08	0x04	/* Parity error in bits 8-15. (ro) */
#define	SUN4C_MPE_ERR16	0x02	/* Parity error in bits 16-23. (ro) */
#define	SUN4C_MPE_ERR24	0x01	/* Parity error in bits 24-31. (ro) */
#define	SUN4C_MPE_ERRS	0x0F	/* Bit mask for the error bits. (ro) */

#endif /* !(_SPARC_MEMREG_H) */
