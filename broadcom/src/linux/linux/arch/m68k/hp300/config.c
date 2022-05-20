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
 *  linux/arch/m68k/hp300/config.c
 *
 *  Copyright (C) 1998 Philip Blundell <philb@gnu.org>
 *
 *  This file contains the HP300-specific initialisation code.  It gets
 *  called by setup.c.
 */

#include <linux/config.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/kd.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <asm/machdep.h>
#include <asm/blinken.h>
#include <asm/hwtest.h>                           /* hwreg_present() */

#include "ints.h"
#include "time.h"

extern void hp300_reset(void);
extern void (*hp300_default_handler[])(int, void *, struct pt_regs *);
extern int hp300_get_irq_list(char *buf);

#ifdef CONFIG_VT
extern int hp300_keyb_init(void);
static int hp300_kbdrate(struct kbd_repeat *k)
{
  return 0;
}
#endif

#ifdef CONFIG_HEARTBEAT
static void hp300_pulse(int x)
{
   if (x)
      blinken_leds(0xfe);
   else
      blinken_leds(0xff);
}
#endif

static void hp300_get_model(char *model)
{
  strcpy(model, "HP9000/300");
}

void __init config_hp300(void)
{
  mach_sched_init      = hp300_sched_init;
#ifdef CONFIG_VT
  mach_keyb_init       = hp300_keyb_init;
  mach_kbdrate         = hp300_kbdrate;
#endif
  mach_init_IRQ        = hp300_init_IRQ;
  mach_request_irq     = hp300_request_irq;
  mach_free_irq        = hp300_free_irq;
  mach_get_model       = hp300_get_model;
  mach_get_irq_list    = hp300_get_irq_list;
  mach_gettimeoffset   = hp300_gettimeoffset;
  mach_default_handler = &hp300_default_handler;
  mach_reset           = hp300_reset;
#ifdef CONFIG_HEARTBEAT
  mach_heartbeat       = hp300_pulse;
#endif
#ifdef CONFIG_DUMMY_CONSOLE
  conswitchp	       = &dummy_con;
#endif
  mach_max_dma_address = 0xffffffff;
}
