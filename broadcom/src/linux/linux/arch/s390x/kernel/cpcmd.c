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
 *  arch/s390/kernel/cpcmd.c
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 */

#include <linux/string.h>
#include <linux/spinlock.h>
#include <asm/cpcmd.h>
#include <asm/ebcdic.h>
#include <asm/system.h>

static spinlock_t cpcmd_lock = SPIN_LOCK_UNLOCKED;
static char cpcmd_buf[128];

void cpcmd(char *cmd, char *response, int rlen)
{
        const int mask = 0x40000000L;
	unsigned long flags;
        int cmdlen;

	spin_lock_irqsave(&cpcmd_lock, flags);
        cmdlen = strlen(cmd);
        strcpy(cpcmd_buf, cmd);
        ASCEBC(cpcmd_buf, cmdlen);

        if (response != NULL && rlen > 0) {
                asm volatile ("   lrag  2,0(%0)\n"
                              "   lgr   4,%1\n"
                              "   o     4,%4\n"
                              "   lrag  3,0(%2)\n"
                              "   lgr   5,%3\n"
                              "   sam31\n"
                              "   .long 0x83240008 # Diagnose 83\n"
                              "   sam64"
                              : /* no output */
                              : "a" (cpcmd_buf), "d" (cmdlen),
                                "a" (response), "d" (rlen), "m" (mask)
                              : "2", "3", "4", "5" );
                EBCASC(response, rlen);
        } else {
                asm volatile ("   lrag  2,0(%0)\n"
                              "   lgr   3,%1\n"
                              "   sam31\n"
                              "   .long 0x83230008 # Diagnose 83\n"
                              "   sam64"
                              : /* no output */
                              : "a" (cpcmd_buf), "d" (cmdlen)
                              : "2", "3"  );
        }
	spin_unlock_irqrestore(&cpcmd_lock, flags);
}

