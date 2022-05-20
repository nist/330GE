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
#ifndef _LMC_DEBUG_H_
#define _LMC_DEBUG_H_

#ifdef DEBUG
#ifdef LMC_PACKET_LOG
#define LMC_CONSOLE_LOG(x,y,z) lmcConsoleLog((x), (y), (z))
#else
#define LMC_CONSOLE_LOG(x,y,z)
#endif
#else
#define LMC_CONSOLE_LOG(x,y,z)
#endif



/* Debug --- Event log definitions --- */
/* EVENTLOGSIZE*EVENTLOGARGS needs to be a power of 2 */
#define LMC_EVENTLOGSIZE 1024	/* number of events in eventlog */
#define LMC_EVENTLOGARGS 4		/* number of args for each event */

/* event indicators */
#define LMC_EVENT_XMT           1
#define LMC_EVENT_XMTEND        2
#define LMC_EVENT_XMTINT        3
#define LMC_EVENT_RCVINT        4
#define LMC_EVENT_RCVEND        5
#define LMC_EVENT_INT           6
#define LMC_EVENT_XMTINTTMO     7
#define LMC_EVENT_XMTPRCTMO     8
#define LMC_EVENT_INTEND        9
#define LMC_EVENT_RESET1       10
#define LMC_EVENT_RESET2       11
#define LMC_EVENT_FORCEDRESET  12
#define LMC_EVENT_WATCHDOG     13
#define LMC_EVENT_BADPKTSURGE  14
#define LMC_EVENT_TBUSY0       15
#define LMC_EVENT_TBUSY1       16


#ifdef DEBUG
extern u_int32_t lmcEventLogIndex;
extern u_int32_t lmcEventLogBuf[LMC_EVENTLOGSIZE * LMC_EVENTLOGARGS];
#define LMC_EVENT_LOG(x, y, z) lmcEventLog((x), (y), (z))
#else
#define LMC_EVENT_LOG(x,y,z)
#endif /* end ifdef _DBG_EVENTLOG */

void lmcConsoleLog(char *type, unsigned char *ucData, int iLen);
void lmcEventLog (u_int32_t EventNum, u_int32_t arg2, u_int32_t arg3);
inline void lmc_trace(struct net_device *dev, char *msg);

#endif
