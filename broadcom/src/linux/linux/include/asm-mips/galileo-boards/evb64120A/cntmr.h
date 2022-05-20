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
/* cntmr.h - Timer/Counter interface header file */

/* Copyright - Galileo technology */

#ifndef __INCtimerCounterDrvh
#define __INCtimerCounterDrvh

/* includes */

#include "core.h"

/* defines */

#define FIRST_CNTMR   0
#define LAST_CNTMR    3

#define CNTMR0_READ(pData)\
        GT_REG_READ(CNTMR0, pData)

#define CNTMR1_READ(pData)\
        GT_REG_READ(CNTMR1, pData)

#define CNTMR2_READ(pData)\
        GT_REG_READ(CNTMR2, pData)

#define CNTMR3_READ(pData)\
        GT_REG_READ(CNTMR3, pData)

/* typedefs */

typedef enum counterTimer{CNTMR_0,CNTMR_1,CNTMR_2,CNTMR_3} CNTMR_NUM;
typedef enum cntTmrOpModes{COUNTER, TIMER} CNT_TMR_OP_MODES;

bool    cntTmrLoad(unsigned int countNum, unsigned int value);
bool    cntTmrSetMode(CNTMR_NUM countNum, CNT_TMR_OP_MODES opMode);
bool    cntTmrEnable(CNTMR_NUM countNum);
bool    cntTmrStart (CNTMR_NUM countNum,unsigned int countValue,
                   CNT_TMR_OP_MODES opMode);
unsigned int    cntTmrDisable(CNTMR_NUM countNum);
unsigned int    cntTmrRead(CNTMR_NUM countNum);

#endif /* __INCtimerCounterDrvh */
