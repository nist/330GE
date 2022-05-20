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
 * arch/m68k/sun3/intersil.c
 *
 * basic routines for accessing the intersil clock within the sun3 machines
 *
 * started 11/12/1999 Sam Creasey
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/kernel.h>
#include <linux/rtc.h>

#include <asm/system.h>
#include <asm/rtc.h>
#include <asm/intersil.h>


/* bits to set for start/run of the intersil */
#define STOP_VAL (INTERSIL_STOP | INTERSIL_INT_ENABLE | INTERSIL_24H_MODE)
#define START_VAL (INTERSIL_RUN | INTERSIL_INT_ENABLE | INTERSIL_24H_MODE)

/* does this need to be implemented? */
unsigned long sun3_gettimeoffset(void)
{ 
  return 1;
}

void sun3_gettod (int *yearp, int *monp, int *dayp,
                   int *hourp, int *minp, int *secp)
{
	unsigned char wday;
	volatile struct intersil_dt* todintersil;
	unsigned long flags;

        todintersil = (struct intersil_dt *) &intersil_clock->counter;

	save_and_cli(flags);

	intersil_clock->cmd_reg = STOP_VAL;

	*secp  = todintersil->csec;
        *hourp = todintersil->hour;
        *minp  = todintersil->minute;
        *secp  = todintersil->second; 
        *monp  = todintersil->month;
        *dayp  = todintersil->day;
        *yearp = todintersil->year+68; /* The base year for sun3 is 1968 */
	wday = todintersil->weekday;

	intersil_clock->cmd_reg = START_VAL;

	restore_flags(flags);
}


/* get/set hwclock */

int sun3_hwclk(int set, struct rtc_time *t)
{
	volatile struct intersil_dt *todintersil;
	unsigned long flags;

        todintersil = (struct intersil_dt *) &intersil_clock->counter;

	save_and_cli(flags);

	intersil_clock->cmd_reg = STOP_VAL;

	/* set or read the clock */
	if(set) {
		todintersil->csec = 0;
		todintersil->hour = t->tm_hour;
		todintersil->minute = t->tm_min;
		todintersil->second = t->tm_sec;
		todintersil->month = t->tm_mon;
		todintersil->day = t->tm_mday;
		todintersil->year = t->tm_year - 68;
		todintersil->weekday = t->tm_wday;
	} else {
		/* read clock */
		t->tm_sec = todintersil->csec;
		t->tm_hour = todintersil->hour;
		t->tm_min = todintersil->minute;
		t->tm_sec = todintersil->second;
		t->tm_mon = todintersil->month;
		t->tm_mday = todintersil->day;
		t->tm_year = todintersil->year + 68;
		t->tm_wday = todintersil->weekday;
	}

	intersil_clock->cmd_reg = START_VAL;

	restore_flags(flags);

	return 0;

}

