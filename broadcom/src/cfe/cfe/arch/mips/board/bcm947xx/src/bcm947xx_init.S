/*  *********************************************************************
    *  BCM947XX Board Support Package
    *
    *  Board-specific initialization		File: BCM947XX_INIT.S
    *
    *  This module contains the assembly-language part of the init
    *  code for this board support package.  The routine
    *  "board_earlyinit" lives here.
    *
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *
    *********************************************************************
    *
    *  Copyright 2000,2001,2002,2003
    *  Broadcom Corporation. All rights reserved.
    *
    *  This software is furnished under license and may be used and
    *  copied only in accordance with the following terms and
    *  conditions.  Subject to these conditions, you may download,
    *  copy, install, use, modify and distribute modified or unmodified
    *  copies of this software in source and/or binary form.  No title
    *  or ownership is transferred hereby.
    *
    *  1) Any source code used, modified or distributed must reproduce
    *     and retain this copyright notice and list of conditions
    *     as they appear in the source file.
    *
    *  2) No right is granted to use any trade name, trademark, or
    *     logo of Broadcom Corporation.  The "Broadcom Corporation"
    *     name may not be used to endorse or promote products derived
    *     from this software without the prior written permission of
    *     Broadcom Corporation.
    *
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT,
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */

#include "sbconfig.h"
#include "bcmdevs.h"
#include "sbchipc.h"
#include "sbmips.h"
#include "bsp_config.h"
#include "mipsmacros.h"

	.text


/*  *********************************************************************
    *  BOARD_EARLYINIT()
    *
    *  Initialize board registers.  This is the earliest
    *  time the BSP gets control.  This routine cannot assume that
    *  memory is operational, and therefore all code in this routine
    *  must run from registers only.  The $ra register must not
    *  be modified, as it contains the return address.
    *
    *  This routine will be called from uncached space, before
    *  the caches are initialized.  If you want to make
    *  subroutine calls from here, you must use the CALLKSEG1 macro.
    *
    *  Among other things, this is where the GPIO registers get
    *  programmed to make on-board LEDs function, or other startup
    *  that has to be done before anything will work.
    *
    *  Input parameters:
    *  	   nothing
    *
    *  Return value:
    *  	   nothing
    ********************************************************************* */

/*
 * Define TRADDR to be an (uncached) addres to store the "leds" for tracing
 * This for example would use chipcommon's flash address register:
 * #define TRADDR		0xb8000044
 */


LEAF(board_earlyinit)

	# XXX:the following code snipet sets clk frequency to 200M
	# correct pll clk freq to real speed in the 5350 case.
	# It is Ugly...but

	li	a2,PHYS_TO_K1(SB_ENUM_BASE)	# Is there a chipcommon core?
	lw	t1,(SBCONFIGOFF + SBIDHIGH)(a2)
	and	t1,t1,SBIDH_CC_MASK
	srl	t1,t1,SBIDH_CC_SHIFT
	bne	t1,SB_CC,_move_ahead
	nop
	
	li	a3,BCM5350_CHIP_ID		# 5350 ChipID
	lw	t1,CC_CHIPID(a2)		# ChipID register
	li	t2,CID_ID_MASK			# chip id is bit 0-15
	and	t2,t1,t2
	bne	t2,a3,_move_ahead		# if not 5350 then skip
	nop

	li	t2,CID_PKG_MASK			# if it is a vsim 5350, also skip
	and	t2,t1,t2
	li	a3,(HDLSIM5350_PKG_ID << CID_PKG_SHIFT)
	beq	t2,a3,_move_ahead		# if pkg opt 1 then skip
	nop

	li	a3,CLKC_5350_N
	lw	t1,CC_CLKC_N(a2)
	beq	a3,t1,_move_ahead	# move ahead if clk freq set correctly
	nop
	sw	a3,CC_CLKC_N(a2)	# set control N1 to select 6
	li	t1,1
	sw	t1,CC_WATCHDOG(a2)	# set WatchDog Reset
1:	b	1b
	nop

_move_ahead:
	li	a3,BCM5354_CHIP_ID		# 5354 ChipID
	and	t2,t1,t2
	bne	t2,a3,_move_ahead2		# if not 5354 then skip
	nop 
	
	li	t1,PHYS_TO_K1(SB_ENUM_BASE)	# CC base

	li      t2, 0x1
	SR      t2, PMU_REG_CONTROL_ADDR(t1)
	LR      t2, PMU_REG_CONTROL_DATA(t1)
	li 	t3, 0x6800000
	beq	t3, t2,_move_ahead1	# move ahead if switcher freq set correctly
	SR      t3, PMU_REG_CONTROL_DATA(t1)
	
	/* 
	 * Trim the output voltage of the 1.2V BB switcher and 2.5V
	 * regulator to the correct value.
	 */
	li	t2,0x0
	sw	t2,PMU_REG_CONTROL_ADDR(a2)
	li 	t3,0x2000			# Reduce the output voltage of
	sw	t3,PMU_REG_CONTROL_DATA(a2)	# BB switcher to get 1.2V
	li	t2,0x3
	sw	t2,PMU_REG_CONTROL_ADDR(a2)
	li 	t3,0x02000000			# Increase the output voltage
	sw	t3,PMU_REG_CONTROL_DATA(a2)	# of VDDP LDO to get 2.5V

	li      t2, 0x1
	SR      t2, PMU_PLL_CONTROL_ADDR(t1)
	LR      t2, PMU_REG_CONTROL_DATA(t1)
	li 	t3, 0x66666602
	beq	t3, t2,_move_ahead1	# move ahead if PLL is set correctly

	li      t2, 0x7ffff
	SR      t2, PMU_MIN_RES_MASK(t1)	# Disable base band PLL
	SR      t2, PMU_MAX_RES_MASK(t1)
	nop

	li      t2, 0x1				# Set the PLL mode properly
	SR      t2, PMU_PLL_CONTROL_ADDR(t1)
	li      t2, 0x66666602
	SR      t2, PMU_PLL_CONTROL_DATA(t1)


	li	t1,1
	sw	t1,CC_WATCHDOG(a2)		# set WatchDog Reset
1:	b	1b
	nop

_move_ahead1:
	li	t1,PHYS_TO_K1(SB_ENUM_BASE)	# CC base
	li      t2, 0xfffff
	SR      t2, PMU_MIN_RES_MASK(t1)	# Enable Base band PLL
	SR      t2, PMU_MAX_RES_MASK(t1)

	#Init code for FF4 space without TLB, enabling RAC 
	li 	t0, 0x1FA0000C 		# Set up CBR to 0x1FAx_xxxx
	mtc0 	t0, $22,6
	li 	t1, 0x1FA00000
	lw 	t2, 0x14(t1)
	or 	t3, t2,0xC0000000 	# enable ffxx_xxxx space # without programming TLB
	sw 	t3, 0x14(t1) 
	li 	t0, 0xFF40000C  	# change CBR to FF4x_xxxx
	mtc0 	t0, $22,6

_move_ahead2:
#ifdef	TRADDR
	li	t0,TRADDR
	li	t1,0xaa55aa55
	sw	t1,0(t0)
#endif
	j	ra

END(board_earlyinit)

/*  *********************************************************************
    *  BOARD_DRAMINFO
    *
    *  Return the address of the DRAM information table
    *
    *  Input parameters:
    *  	   nothing
    *
    *  Return value:
    *  	   v0 - DRAM info table, return 0 to use default table
    ********************************************************************* */

LEAF(board_draminfo)

	move	v0,zero		# auto configure
	j	ra

END(board_draminfo)

/*  *********************************************************************
    *  BOARD_SETLEDS(x)
    *
    *  Set LEDs for boot-time progress indication.  Not used if
    *  the board does not have progress LEDs.  This routine
    *  must not call any other routines, since it may be invoked
    *  either from KSEG0 or KSEG1 and it may be invoked
    *  whether or not the icache is operational.
    *
    *  Input parameters:
    *  	   a0 - LED value (8 bits per character, 4 characters)
    *
    *  Return value:
    *  	   nothing
    ********************************************************************* */

LEAF(board_setleds)

#ifdef	TRADDR
	li	t0,TRADDR
	sw	a0,0(t0)
#endif
	j	ra

END(board_setleds)
