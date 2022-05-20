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
 * IR port driver for the Cirrus Logic EP7211 processor.
 *
 * Copyright 2001, Blue Mug Inc.  All rights reserved.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/init.h>

#include <net/irda/irda.h>
#include <net/irda/irmod.h>
#include <net/irda/irda_device.h>

#include <asm/io.h>
#include <asm/hardware.h>

#define MIN_DELAY 25      /* 15 us, but wait a little more to be sure */
#define MAX_DELAY 10000   /* 1 ms */

static void ep7211_ir_open(dongle_t *self, struct qos_info *qos);
static void ep7211_ir_close(dongle_t *self);
static int  ep7211_ir_change_speed(struct irda_task *task);
static int  ep7211_ir_reset(struct irda_task *task);

static struct dongle_reg dongle = {
	Q_NULL,
	IRDA_EP7211_IR,
	ep7211_ir_open,
	ep7211_ir_close,
	ep7211_ir_reset,
	ep7211_ir_change_speed,
};

static void ep7211_ir_open(dongle_t *self, struct qos_info *qos)
{
	unsigned int syscon1, flags;

	save_flags(flags); cli();

	/* Turn on the SIR encoder. */
	syscon1 = clps_readl(SYSCON1);
	syscon1 |= SYSCON1_SIREN;
	clps_writel(syscon1, SYSCON1);


	restore_flags(flags);

	MOD_INC_USE_COUNT;
}

static void ep7211_ir_close(dongle_t *self)
{
	unsigned int syscon1, flags;

	save_flags(flags); cli();

	/* Turn off the SIR encoder. */
	syscon1 = clps_readl(SYSCON1);
	syscon1 &= ~SYSCON1_SIREN;
	clps_writel(syscon1, SYSCON1);


	restore_flags(flags);

	MOD_DEC_USE_COUNT;
}

/*
 * Function ep7211_ir_change_speed (task)
 *
 *    Change speed of the EP7211 I/R port. We don't really have to do anything
 *    for the EP7211 as long as the rate is being changed at the serial port
 *    level.
 */
static int ep7211_ir_change_speed(struct irda_task *task)
{
	irda_task_next_state(task, IRDA_TASK_DONE);
	return 0;
}

/*
 * Function ep7211_ir_reset (task)
 *
 *      Reset the EP7211 I/R. We don't really have to do anything.
 *
 */
static int ep7211_ir_reset(struct irda_task *task)
{
	irda_task_next_state(task, IRDA_TASK_DONE);
	return 0;
}

/*
 * Function ep7211_ir_init(void)
 *
 *    Initialize EP7211 I/R module
 *
 */
int __init ep7211_ir_init(void)
{
	return irda_device_register_dongle(&dongle);
}

/*
 * Function ep7211_ir_cleanup(void)
 *
 *    Cleanup EP7211 I/R module
 *
 */
static void __exit ep7211_ir_cleanup(void)
{
	irda_device_unregister_dongle(&dongle);
}

MODULE_AUTHOR("Jon McClintock <jonm@bluemug.com>");
MODULE_DESCRIPTION("EP7211 I/R driver");
MODULE_LICENSE("GPL");
		
#ifdef MODULE
module_init(ep7211_ir_init);
#endif /* MODULE */
module_exit(ep7211_ir_cleanup);