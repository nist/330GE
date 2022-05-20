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
 * BK Id: %F% %I% %G% %U% %#%
 */
#include <linux/config.h> /* CONFIG_HEARTBEAT */
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>

#include <asm/machdep.h>
#include <asm/io.h>

#include <linux/timex.h>

unsigned long m68k_get_rtc_time(void)
{
	unsigned int year, mon, day, hour, min, sec;

	extern void arch_gettod(int *year, int *mon, int *day, int *hour,
				int *min, int *sec);

	arch_gettod (&year, &mon, &day, &hour, &min, &sec);

	if ((year += 1900) < 1970)
		year += 100;

	return mktime(year, mon, day, hour, min, sec);
}

int m68k_set_rtc_time(unsigned long nowtime)
{
  if (mach_set_clock_mmss)
    return mach_set_clock_mmss (nowtime);
  return -1;
}

void apus_heartbeat (void)
{
#ifdef CONFIG_HEARTBEAT
	static unsigned cnt = 0, period = 0, dist = 0;

	if (cnt == 0 || cnt == dist)
                mach_heartbeat( 1 );
	else if (cnt == 7 || cnt == dist+7)
                mach_heartbeat( 0 );

	if (++cnt > period) {
                cnt = 0;
                /* The hyperbolic function below modifies the heartbeat period
                 * length in dependency of the current (5min) load. It goes
                 * through the points f(0)=126, f(1)=86, f(5)=51,
                 * f(inf)->30. */
                period = ((672<<FSHIFT)/(5*avenrun[0]+(7<<FSHIFT))) + 30;
                dist = period / 4;
	}
#endif
}
