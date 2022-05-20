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
/*!**************************************************************************
*!                                                            
*! MACROS:
*!   IO_MASK(reg,field)
*!   IO_STATE(reg,field,state)
*!   IO_EXTRACT(reg,field,val)
*!   IO_STATE_VALUE(reg,field,state)
*!   IO_BITNR(reg,field)
*!   IO_WIDTH(reg,field)
*!   IO_FIELD(reg,field,val)
*!   IO_RD(reg)
*!   All moderegister addresses and fields of these.
*!
*!**************************************************************************/

#ifndef __sv_addr_ag_h__
#define __sv_addr_ag_h__


#define __test_sv_addr__ 0

/*------------------------------------------------------------
!* General macros to manipulate moderegisters.
!*-----------------------------------------------------------*/

/* IO_MASK returns a mask for a specified bitfield in a register.
   Note that this macro doesn't work when field width is 32 bits. */
#define IO_MASK(reg,field) \
    ( ( ( 1 << reg##__##field##__WIDTH ) - 1 ) << reg##__##field##__BITNR )

/* IO_STATE returns a constant corresponding to a one of the symbolic
   states that the bitfield can have. (Shifted to correct position)  */
#define IO_STATE(reg,field,state) \
    ( reg##__##field##__##state << reg##__##field##__BITNR )

/* IO_EXTRACT returns the masked and shifted value corresponding to the
   bitfield can have. */
#define IO_EXTRACT(reg,field,val) ( (( ( ( 1 << reg##__##field##__WIDTH ) \
     - 1 ) << reg##__##field##__BITNR ) & (val)) >> reg##__##field##__BITNR )

/* IO_STATE_VALUE returns a constant corresponding to a one of the symbolic
   states that the bitfield can have. (Not shifted)  */
#define IO_STATE_VALUE(reg,field,state) ( reg##__##field##__##state )

/* IO_FIELD shifts the val parameter to be aligned with the bitfield
   specified. */
#define IO_FIELD(reg,field,val) ((val) << reg##__##field##__BITNR)

/* IO_BITNR returns the starting bitnumber of a bitfield. Bit 0 is
   LSB and the returned bitnumber is LSB of the field. */
#define IO_BITNR(reg,field) (reg##__##field##__BITNR)

/* IO_WIDTH returns the width, in bits, of a bitfield. */
#define IO_WIDTH(reg,field) (reg##__##field##__WIDTH)

/*--- Obsolete. Kept for backw compatibility. ---*/
/* Reads (or writes) a byte/uword/udword from the specified mode
   register. */
#define IO_RD(reg) (*(volatile u32*)(reg))
#define IO_RD_B(reg) (*(volatile u8*)(reg))
#define IO_RD_W(reg) (*(volatile u16*)(reg))
#define IO_RD_D(reg) (*(volatile u32*)(reg))

/*------------------------------------------------------------
!* Start addresses of the different memory areas.
!*-----------------------------------------------------------*/

#define MEM_CSE0_START (0x00000000)
#define MEM_CSE1_START (0x04000000)
#define MEM_CSR0_START (0x08000000)
#define MEM_CSR1_START (0x0c000000)
#define MEM_CSP0_START (0x10000000)
#define MEM_CSP1_START (0x14000000)
#define MEM_CSP2_START (0x18000000)
#define MEM_CSP3_START (0x1c000000)
#define MEM_CSP4_START (0x20000000)
#define MEM_CSP5_START (0x24000000)
#define MEM_CSP6_START (0x28000000)
#define MEM_CSP7_START (0x2c000000)
#define MEM_DRAM_START (0x40000000)

#define MEM_NON_CACHEABLE (0x80000000)

/*------------------------------------------------------------
!* Type casts used in mode register macros, making pointer
!* dereferencing possible. Empty in assembler.
!*-----------------------------------------------------------*/

#ifndef __ASSEMBLER__
# define  IO_TYPECAST_UDWORD  (volatile u32*)
# define  IO_TYPECAST_RO_UDWORD  (const volatile u32*)
# define  IO_TYPECAST_UWORD  (volatile u16*)
# define  IO_TYPECAST_RO_UWORD  (const volatile u16*)
# define  IO_TYPECAST_BYTE  (volatile u8*)
# define  IO_TYPECAST_RO_BYTE  (const volatile u8*)
#else
# define  IO_TYPECAST_UDWORD
# define  IO_TYPECAST_RO_UDWORD
# define  IO_TYPECAST_UWORD
# define  IO_TYPECAST_RO_UWORD
# define  IO_TYPECAST_BYTE
# define  IO_TYPECAST_RO_BYTE
#endif

/*------------------------------------------------------------*/

#include "sv_addr.agh"

#if __test_sv_addr__
/* IO_MASK( R_BUS_CONFIG , CE ) */
IO_MASK( R_WAITSTATES , SRAM_WS )
IO_MASK( R_TEST , W32 )

IO_STATE( R_BUS_CONFIG, CE, DISABLE )
IO_STATE( R_BUS_CONFIG, CE, ENABLE )

IO_STATE( R_DRAM_TIMING, REF, IVAL2 )

IO_MASK( R_DRAM_TIMING, REF )

IO_MASK( R_EXT_DMA_0_STAT, TFR_COUNT ) >> IO_BITNR( R_EXT_DMA_0_STAT, TFR_COUNT )

IO_RD(R_EXT_DMA_0_STAT) & IO_MASK( R_EXT_DMA_0_STAT, S ) 
   == IO_STATE( R_EXT_DMA_0_STAT, S, STARTED )
#endif


#endif  /* ifndef __sv_addr_ag_h__ */
