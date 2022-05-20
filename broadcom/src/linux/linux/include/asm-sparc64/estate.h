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
/* $Id: estate.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_ESTATE_H
#define _SPARC64_ESTATE_H

/* UltraSPARC-III E-cache Error Enable */
#define ESTATE_ERROR_FMT	0x0000000000040000 /* Force MTAG ECC		*/
#define ESTATE_ERROR_FMESS	0x000000000003c000 /* Forced MTAG ECC val	*/
#define ESTATE_ERROR_FMD	0x0000000000002000 /* Force DATA ECC		*/
#define ESTATE_ERROR_FDECC	0x0000000000001ff0 /* Forced DATA ECC val	*/
#define ESTATE_ERROR_UCEEN	0x0000000000000008 /* See below			*/
#define ESTATE_ERROR_NCEEN	0x0000000000000002 /* See below			*/
#define ESTATE_ERROR_CEEN	0x0000000000000001 /* See below			*/

/* UCEEN enables the fast_ECC_error trap for: 1) software correctable E-cache
 * errors 2) uncorrectable E-cache errors.  Such events only occur on reads
 * of the E-cache by the local processor for: 1) data loads 2) instruction
 * fetches 3) atomic operations.  Such events _cannot_ occur for: 1) merge
 * 2) writeback 2) copyout.  The AFSR bits assosciated with these traps are
 * UCC and UCU.
 */

/* NCEEN enables instruction_access_error, data_access_error, and ECC_error traps
 * for uncorrectable ECC errors and system errors.
 *
 * Uncorrectable system bus data error or MTAG ECC error, system bus TimeOUT,
 * or system bus BusERR:
 * 1) As the result of an instruction fetch, will generate instruction_access_error
 * 2) As the result of a load etc. will generate data_access_error.
 * 3) As the result of store merge completion, writeback, or copyout will
 *    generate a disrupting ECC_error trap.
 * 4) As the result of such errors on instruction vector fetch can generate any
 *    of the 3 trap types.
 *
 * The AFSR bits assosciated with these traps are EMU, EDU, WDU, CPU, IVU, UE,
 * BERR, and TO.
 */

/* CEEN enables the ECC_error trap for hardware corrected ECC errors.  System bus
 * reads resulting in a hardware corrected data or MTAG ECC error will generate an
 * ECC_error disrupting trap with this bit enabled.
 *
 * This same trap will also be generated when a hardware corrected ECC error results
 * during store merge, writeback, and copyout operations.
 */

/* In general, if the trap enable bits above are disabled the AFSR bits will still
 * log the events even though the trap will not be generated by the processor.
 */

#endif /* _SPARC64_ESTATE_H */
