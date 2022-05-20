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
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: br_private_timer.h,v 1.1.1.1 2007/01/25 12:52:11 jiahao_jhou Exp $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#ifndef _BR_PRIVATE_TIMER_H
#define _BR_PRIVATE_TIMER_H

struct br_timer
{
	int running;
	unsigned long expires;
};

extern __inline__ void br_timer_clear(struct br_timer *t)
{
	t->running = 0;
}

extern __inline__ unsigned long br_timer_get_residue(struct br_timer *t)
{
	if (t->running)
		return jiffies - t->expires;

	return 0;
}

extern __inline__ void br_timer_set(struct br_timer *t, unsigned long x)
{
	t->expires = x;
	t->running = 1;
}

extern __inline__ int br_timer_is_running(struct br_timer *t)
{
	return t->running;
}

extern __inline__ int br_timer_has_expired(struct br_timer *t, unsigned long to)
{
	return t->running && time_after_eq(jiffies, t->expires + to);
}


#endif
