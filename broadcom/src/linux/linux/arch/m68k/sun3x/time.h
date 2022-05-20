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
#ifndef SUN3X_TIME_H
#define SUN3X_TIME_H

extern void sun3x_gettod (int *yearp, int *monp, int *dayp,
                   int *hourp, int *minp, int *secp);
extern int sun3x_hwclk(int set, struct rtc_time *t);
unsigned long sun3x_gettimeoffset (void);
void sun3x_sched_init(void (*vector)(int, void *, struct pt_regs *));

struct mostek_dt {
	volatile unsigned char csr;
	volatile unsigned char sec;
	volatile unsigned char min;
	volatile unsigned char hour;
	volatile unsigned char wday;
	volatile unsigned char mday;
	volatile unsigned char month;
	volatile unsigned char year;
};

#endif
