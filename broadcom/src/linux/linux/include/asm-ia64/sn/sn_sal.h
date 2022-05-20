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
#ifndef _ASM_IA64_SN_SN_SAL_H
#define _ASM_IA64_SN_SN_SAL_H

/*
 * System Abstraction Layer definitions for IA64
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2000-2002 Silicon Graphics, Inc.  All rights reserved.
 */


#include <asm/sal.h>
#include <asm/sn/sn_cpuid.h>


// SGI Specific Calls
#define  SN_SAL_POD_MODE                           0x02000001
#define  SN_SAL_SYSTEM_RESET                       0x02000002
#define  SN_SAL_PROBE                              0x02000003
#define  SN_SAL_GET_CONSOLE_NASID                  0x02000004
#define	 SN_SAL_GET_KLCONFIG_ADDR		   0x02000005
#define  SN_SAL_LOG_CE				   0x02000006
#define  SN_SAL_REGISTER_CE			   0x02000007
#define  SN_SAL_CONSOLE_PUTC                       0x02000021
#define  SN_SAL_CONSOLE_GETC                       0x02000022
#define  SN_SAL_CONSOLE_PUTS                       0x02000023
#define  SN_SAL_CONSOLE_GETS                       0x02000024
#define  SN_SAL_CONSOLE_GETS_TIMEOUT               0x02000025
#define  SN_SAL_CONSOLE_POLL                       0x02000026
#define  SN_SAL_CONSOLE_INTR                       0x02000027

/*
 * Service-specific constants
 */
#define SAL_CONSOLE_INTR_IN     0       /* manipulate input interrupts */
#define SAL_CONSOLE_INTR_OUT    1       /* manipulate output low-water
                                         * interrupts
                                         */
#define SAL_CONSOLE_INTR_OFF    0       /* turn the interrupt off */
#define SAL_CONSOLE_INTR_ON     1       /* turn the interrupt on */




u64 ia64_sn_probe_io_slot(long paddr, long size, void *data_ptr);

/*
 * Returns the master console nasid, if the call fails, return an illegal
 * value.
 */
static inline u64
ia64_sn_get_console_nasid(void)
{
	struct ia64_sal_retval ret_stuff;

	ret_stuff.status = (uint64_t)0;
	ret_stuff.v0 = (uint64_t)0;
	ret_stuff.v1 = (uint64_t)0;
	ret_stuff.v2 = (uint64_t)0;
	SAL_CALL(ret_stuff, SN_SAL_GET_CONSOLE_NASID, 0, 0, 0, 0, 0, 0, 0);

	if (ret_stuff.status < 0)
		return ret_stuff.status;

	/* Master console nasid is in 'v0' */
	return ret_stuff.v0;
}

static inline u64
ia64_sn_get_klconfig_addr(nasid_t nasid)
{
	struct ia64_sal_retval ret_stuff;
	extern u64 klgraph_addr[];
	int cnodeid;

	cnodeid = nasid_to_cnodeid(nasid);
	if (klgraph_addr[cnodeid] == 0) {
		ret_stuff.status = (uint64_t)0;
		ret_stuff.v0 = (uint64_t)0;
		ret_stuff.v1 = (uint64_t)0;
		ret_stuff.v2 = (uint64_t)0;
		SAL_CALL(ret_stuff, SN_SAL_GET_KLCONFIG_ADDR, (u64)nasid, 0, 0, 0, 0, 0, 0);

		/*
	 	* We should panic if a valid cnode nasid does not produce
	 	* a klconfig address.
	 	*/
		if (ret_stuff.status != 0) {
			panic("ia64_sn_get_klconfig_addr: Returned error %lx\n", ret_stuff.status);
		}

		klgraph_addr[cnodeid] = ret_stuff.v0;
	}
	return(klgraph_addr[cnodeid]);
}

/*
 * Returns the next console character.
 */
static inline u64
ia64_sn_console_getc(int *ch)
{
	struct ia64_sal_retval ret_stuff;

	ret_stuff.status = (uint64_t)0;
	ret_stuff.v0 = (uint64_t)0;
	ret_stuff.v1 = (uint64_t)0;
	ret_stuff.v2 = (uint64_t)0;
	SAL_CALL(ret_stuff, SN_SAL_CONSOLE_GETC, 0, 0, 0, 0, 0, 0, 0);

	/* character is in 'v0' */
	*ch = (int)ret_stuff.v0;

	return ret_stuff.status;
}

/*
 * Sends the given character to the console.
 */
static inline u64
ia64_sn_console_putc(char ch)
{
	struct ia64_sal_retval ret_stuff;

	ret_stuff.status = (uint64_t)0;
	ret_stuff.v0 = (uint64_t)0;
	ret_stuff.v1 = (uint64_t)0;
	ret_stuff.v2 = (uint64_t)0;
	SAL_CALL(ret_stuff, SN_SAL_CONSOLE_PUTC, (uint64_t)ch, 0, 0, 0, 0, 0, 0);

	return ret_stuff.status;
}

/*
 * Checks for console input.
 */
static inline u64
ia64_sn_console_check(int *result)
{
	struct ia64_sal_retval ret_stuff;

	ret_stuff.status = (uint64_t)0;
	ret_stuff.v0 = (uint64_t)0;
	ret_stuff.v1 = (uint64_t)0;
	ret_stuff.v2 = (uint64_t)0;
	SAL_CALL(ret_stuff, SN_SAL_CONSOLE_POLL, 0, 0, 0, 0, 0, 0, 0);

	/* result is in 'v0' */
	*result = (int)ret_stuff.v0;

	return ret_stuff.status;
}

#endif /* _ASM_IA64_SN_SN_SAL_H */
