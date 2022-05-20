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
/* $Id: fasttimer.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 * linux/include/asm-cris/fasttimer.h
 *
 * Fast timers for ETRAX100LX
 * This may be useful in other OS than Linux so use 2 space indentation...
 * Copyright (C) 2000, 2002 Axis Communications AB
 */
#include <linux/config.h>
#include <linux/time.h> /* struct timeval */
#include <linux/timex.h>

/* The timer0 values gives 52us resolution (1/19200) or higher 
 * but interrupts at HZ 
 */
/* We use timer1 to generate interrupts at desired times. */

#ifdef CONFIG_ETRAX_FAST_TIMER

typedef void fast_timer_function_type(unsigned long);

struct fast_timer{ /* Close to timer_list */
  struct fast_timer *next;
  struct fast_timer *prev;
  struct timeval tv_set;
  struct timeval tv_expires;
  unsigned long delay_us;
  fast_timer_function_type *function;
  unsigned long data;
  const char *name;
};

void start_one_shot_timer(struct fast_timer *t,
                          fast_timer_function_type *function,
                          unsigned long data,
                          unsigned long delay_us,
                          const char *name);

int del_fast_timer(struct fast_timer * t);
/* return 1 if deleted */


void schedule_usleep(unsigned long us);


void fast_timer_init(void);

#endif
