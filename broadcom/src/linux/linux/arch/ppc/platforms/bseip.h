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
 * BK Id: SCCS/s.bseip.h 1.10 08/17/01 15:23:17 paulus
 */

/*
 * A collection of structures, addresses, and values associated with
 * the Bright Star Engineering ip-Engine board.  Copied from the MBX stuff.
 *
 * Copyright (c) 1998 Dan Malek (dmalek@jlc.net)
 */
#ifndef __MACH_BSEIP_DEFS
#define __MACH_BSEIP_DEFS

#ifndef __ASSEMBLY__
/* A Board Information structure that is given to a program when
 * prom starts it up.
 */
typedef struct bd_info {
	unsigned int	bi_memstart;	/* Memory start address */
	unsigned int	bi_memsize;	/* Memory (end) size in bytes */
	unsigned int	bi_intfreq;	/* Internal Freq, in Hz */
	unsigned int	bi_busfreq;	/* Bus Freq, in Hz */
	unsigned char	bi_enetaddr[6];
	unsigned int	bi_baudrate;
} bd_t;

extern bd_t m8xx_board_info;

/* Memory map is configured by the PROM startup.
 * All we need to get started is the IMMR.
 */
#define IMAP_ADDR		((uint)0xff000000)
#define IMAP_SIZE		((uint)(64 * 1024))
#define PCMCIA_MEM_ADDR		((uint)0x04000000)
#define PCMCIA_MEM_SIZE		((uint)(64 * 1024))
#endif	/* !__ASSEMBLY__ */

/* We don't use the 8259.
*/
#define NR_8259_INTS	0

#endif
