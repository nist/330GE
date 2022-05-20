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
 * Generic linux-input device driver for axis-bearing devices
 *
 * Copyright (c) 2001 Brian S. Julin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *
 * References:
 * HP-HIL Technical Reference Manual.  Hewlett Packard Product No. 45918A
 *
 */

#include <linux/hil.h>
#include <linux/input.h>
#include <linux/serio.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

#define PREFIX "HIL PTR: "
#define HIL_GENERIC_NAME "generic HIL pointer device"

MODULE_AUTHOR("Brian S. Julin <bri@calyx.com>");
MODULE_DESCRIPTION(HIL_GENERIC_NAME " driver");
MODULE_LICENSE("Dual BSD/GPL");


#define TABLET_SIMULATES_MOUSE	/* allow tablet to be used as mouse */
#undef  TABLET_AUTOADJUST	/* auto-adjust valid tablet ranges */


#define HIL_PTR_MAX_LENGTH 16

struct hil_ptr {
	struct input_dev dev;
	struct serio *serio;

	/* Input buffer and index for packets from HIL bus. */
	hil_packet data[HIL_PTR_MAX_LENGTH];
	int idx4; /* four counts per packet */

	/* Raw device info records from HIL bus, see hil.h for fields. */
	char	idd[HIL_PTR_MAX_LENGTH];	/* DID byte and IDD record */
	char	rsc[HIL_PTR_MAX_LENGTH];	/* RSC record */
	char	exd[HIL_PTR_MAX_LENGTH];	/* EXD record */
	char	rnm[HIL_PTR_MAX_LENGTH + 1];	/* RNM record + NULL term. */

	/* Extra device details not contained in struct input_dev. */
	unsigned int nbtn, naxes;
	unsigned int btnmap[7];

	/* Something to sleep around with. */
	struct semaphore sem;
};

/* Process a complete packet after transfer from the HIL */
static void hil_ptr_process_record(struct hil_ptr *ptr)
{
	struct input_dev *dev = &ptr->dev;
	hil_packet *data = ptr->data;
	hil_packet p;
	int idx, i, cnt, laxis;
	int ax16, absdev;

	idx = ptr->idx4/4;
	p = data[idx - 1];

	if ((p & ~HIL_CMDCT_POL) == 
	    (HIL_ERR_INT | HIL_PKT_CMD | HIL_CMD_POL)) goto report;
	if ((p & ~HIL_CMDCT_RPL) == 
	    (HIL_ERR_INT | HIL_PKT_CMD | HIL_CMD_RPL)) goto report;

	/* Not a poll response.  See if we are loading config records. */
	switch (p & HIL_PKT_DATA_MASK) {
	case HIL_CMD_IDD:
		for (i = 0; i < idx; i++)
			ptr->idd[i] = ptr->data[i] & HIL_PKT_DATA_MASK;
		for (; i < HIL_PTR_MAX_LENGTH; i++)
			ptr->idd[i] = 0;
		break;
	case HIL_CMD_RSC:
		for (i = 0; i < idx; i++)
			ptr->rsc[i] = ptr->data[i] & HIL_PKT_DATA_MASK;
		for (; i < HIL_PTR_MAX_LENGTH; i++)
			ptr->rsc[i] = 0;
		break;
	case HIL_CMD_EXD:
		for (i = 0; i < idx; i++)
			ptr->exd[i] = ptr->data[i] & HIL_PKT_DATA_MASK;
		for (; i < HIL_PTR_MAX_LENGTH; i++)
			ptr->exd[i] = 0;
		break;
	case HIL_CMD_RNM:
		for (i = 0; i < idx; i++)
			ptr->rnm[i] = ptr->data[i] & HIL_PKT_DATA_MASK;
		for (; i < HIL_PTR_MAX_LENGTH + 1; i++)
			ptr->rnm[i] = '\0';
		break;
	default:
		/* These occur when device isn't present */
		if (p == (HIL_ERR_INT | HIL_PKT_CMD)) break; 
		/* Anything else we'd like to know about. */
		printk(KERN_WARNING PREFIX "Device sent unknown record %x\n", p);
		break;
	}
	goto out;

 report:
	if ((p & HIL_CMDCT_POL) != idx - 1) {
		printk(KERN_WARNING PREFIX "Malformed poll packet %x (idx = %i)\n", p, idx);
		goto out;
	}

	i = (ptr->data[0] & HIL_POL_AXIS_ALT) ? 3 : 0;
	laxis = ptr->data[0] & HIL_POL_NUM_AXES_MASK;
	laxis += i;

	ax16 = ptr->idd[1] & HIL_IDD_HEADER_16BIT; /* 8 or 16bit resolution */
	absdev = ptr->idd[1] & HIL_IDD_HEADER_ABS; 

	for (cnt = 1; i < laxis; i++) {
		unsigned int lo,hi,val;
		lo = ptr->data[cnt++] & HIL_PKT_DATA_MASK;
		hi = ax16 ? (ptr->data[cnt++] & HIL_PKT_DATA_MASK) : 0;
		if (absdev) {
			val = lo + (hi<<8);
#ifdef TABLET_AUTOADJUST
			if (val < ptr->dev.absmin[ABS_X + i])
				ptr->dev.absmin[ABS_X + i] = val;
			if (val > ptr->dev.absmax[ABS_X + i])
				ptr->dev.absmax[ABS_X + i] = val;
#endif
			if (i%3) val = ptr->dev.absmax[ABS_X + i] - val;
			input_report_abs(dev, ABS_X + i, val);
		} else {
			val = (int) (((int8_t)lo) | ((int8_t)hi<<8));
			if (i%3) val *= -1;
			input_report_rel(dev, REL_X + i, val);
		}
	}

	while (cnt < idx - 1) {
		unsigned int btn;
		int up;
		btn = ptr->data[cnt++];
		up = btn & 1;
		btn &= 0xfe;
		if (btn == 0x8e) {
			continue; /* TODO: proximity == touch? */
		}
		else if ((btn > 0x8c) || (btn < 0x80)) continue;
		btn = (btn - 0x80) >> 1;
		btn = ptr->btnmap[btn];
		input_report_key(dev, btn, !up);
	}
 out:
	ptr->idx4 = 0;
	up(&ptr->sem);
}

static void hil_ptr_process_err(struct hil_ptr *ptr) {
	printk(KERN_WARNING PREFIX "errored HIL packet\n");
	ptr->idx4 = 0;
	up(&ptr->sem);
	return;
}

static void hil_ptr_interrupt(struct serio *serio, 
			      unsigned char data, 
			      unsigned int flags)
{
	struct hil_ptr *ptr;
	hil_packet packet;
	int idx;

	ptr = (struct hil_ptr *)serio->private;
	if (ptr == NULL) {
		BUG();
		return;
	}

	if (ptr->idx4 >= (HIL_PTR_MAX_LENGTH * sizeof(hil_packet))) {
		hil_ptr_process_err(ptr);
		return;
	}
	idx = ptr->idx4/4;
	if (!(ptr->idx4 % 4)) ptr->data[idx] = 0;
	packet = ptr->data[idx];
	packet |= ((hil_packet)data) << ((3 - (ptr->idx4 % 4)) * 8);
	ptr->data[idx] = packet;

	/* Records of N 4-byte hil_packets must terminate with a command. */
	if ((++(ptr->idx4)) % 4) return;
	if ((packet & 0xffff0000) != HIL_ERR_INT) {
		hil_ptr_process_err(ptr);
		return;
	}
	if (packet & HIL_PKT_CMD) 
		hil_ptr_process_record(ptr);
}

static void hil_ptr_disconnect(struct serio *serio)
{
	struct hil_ptr *ptr;

	ptr = (struct hil_ptr *)serio->private;
	if (ptr == NULL) {
		BUG();
		return;
	}

	input_unregister_device(&ptr->dev);
	serio_close(serio);
	kfree(ptr);
}

static void hil_ptr_connect(struct serio *serio, struct serio_dev *dev)
{
	struct hil_ptr	*ptr;
	char		*txt;
	unsigned int	i, naxsets, btntype;
	uint8_t		did, *idd;

	if (serio->type != (SERIO_HIL_MLC | SERIO_HIL)) return;

	if (!(ptr = kmalloc(sizeof(struct hil_ptr), GFP_KERNEL))) return;
	memset(ptr, 0, sizeof(struct hil_ptr));

	if (serio_open(serio, dev)) goto bail0;

	serio->private = ptr;
	ptr->serio = serio;
	ptr->dev.private = ptr;

	init_MUTEX_LOCKED(&(ptr->sem));

	/* Get device info.  MLC driver supplies devid/status/etc. */
	serio->write(serio, 0);
	serio->write(serio, 0);
	serio->write(serio, HIL_PKT_CMD >> 8);
	serio->write(serio, HIL_CMD_IDD);
	down(&(ptr->sem));

	serio->write(serio, 0);
	serio->write(serio, 0);
	serio->write(serio, HIL_PKT_CMD >> 8);
	serio->write(serio, HIL_CMD_RSC);
	down(&(ptr->sem));

	serio->write(serio, 0);
	serio->write(serio, 0);
	serio->write(serio, HIL_PKT_CMD >> 8);
	serio->write(serio, HIL_CMD_RNM);
	down(&(ptr->sem));

	serio->write(serio, 0);
	serio->write(serio, 0);
	serio->write(serio, HIL_PKT_CMD >> 8);
	serio->write(serio, HIL_CMD_EXD);
	down(&(ptr->sem));

	up(&(ptr->sem));

	did = ptr->idd[0];
	idd = ptr->idd + 1;
	txt = "unknown";
	if ((did & HIL_IDD_DID_TYPE_MASK) == HIL_IDD_DID_TYPE_REL) {
		ptr->dev.evbit[0] = BIT(EV_REL);
		txt = "relative";
	}

	if ((did & HIL_IDD_DID_TYPE_MASK) == HIL_IDD_DID_TYPE_ABS) {
		ptr->dev.evbit[0] = BIT(EV_ABS);
		txt = "absolute";
	}
	if (!ptr->dev.evbit[0]) {
		goto bail1;
	}

	ptr->nbtn = HIL_IDD_NUM_BUTTONS(idd);
	if (ptr->nbtn) ptr->dev.evbit[0] |= BIT(EV_KEY);

	naxsets = HIL_IDD_NUM_AXSETS(*idd);
	ptr->naxes = HIL_IDD_NUM_AXES_PER_SET(*idd);

	printk(KERN_INFO PREFIX "HIL pointer device found (did: 0x%02x, axis: %s)\n",
			did, txt);
	printk(KERN_INFO PREFIX "HIL pointer has %i buttons and %i sets of %i axes\n",
			ptr->nbtn, naxsets, ptr->naxes);
	
	btntype = BTN_MISC;
	if ((did & HIL_IDD_DID_ABS_TABLET_MASK) == HIL_IDD_DID_ABS_TABLET)
#ifdef TABLET_SIMULATES_MOUSE
		btntype = BTN_TOUCH;
#else
		btntype = BTN_DIGI;
#endif
	if ((did & HIL_IDD_DID_ABS_TSCREEN_MASK) == HIL_IDD_DID_ABS_TSCREEN)
		btntype = BTN_TOUCH;
		
	if ((did & HIL_IDD_DID_REL_MOUSE_MASK) == HIL_IDD_DID_REL_MOUSE)
		btntype = BTN_MOUSE;

	for (i = 0; i < ptr->nbtn; i++) {
		set_bit(btntype | i, ptr->dev.keybit);
		ptr->btnmap[i] = btntype | i;
	}

	if (btntype == BTN_MOUSE) {
		/* Swap buttons 2 and 3 */
		ptr->btnmap[1] = BTN_MIDDLE;
		ptr->btnmap[2] = BTN_RIGHT;
	}

	if ((did & HIL_IDD_DID_TYPE_MASK) == HIL_IDD_DID_TYPE_REL) {
		for (i = 0; i < ptr->naxes; i++) {
			set_bit(REL_X + i, ptr->dev.relbit);
		}
		for (i = 3; (i < ptr->naxes + 3) && (naxsets > 1); i++) {
			set_bit(REL_X + i, ptr->dev.relbit);
		}
	} else {
		for (i = 0; i < ptr->naxes; i++) {
	  		set_bit(ABS_X + i, ptr->dev.absbit);
			ptr->dev.absmin[ABS_X + i] = 0;
			ptr->dev.absmax[ABS_X + i] = 
				HIL_IDD_AXIS_MAX((ptr->idd + 1), i);
		}
		for (i = 3; (i < ptr->naxes + 3) && (naxsets > 1); i++) {
			set_bit(ABS_X + i, ptr->dev.absbit);
			ptr->dev.absmin[ABS_X + i] = 0;
			ptr->dev.absmax[ABS_X + i] = 
				HIL_IDD_AXIS_MAX((ptr->idd + 1), (i - 3));
		}
#ifdef TABLET_AUTOADJUST
		for (i = 0; i < ABS_MAX; i++) {
			int diff = ptr->dev.absmax[ABS_X + i] / 10;
			ptr->dev.absmin[ABS_X + i] += diff;
			ptr->dev.absmax[ABS_X + i] -= diff;
		}
#endif
	}

	ptr->dev.name = strlen(ptr->rnm) ? ptr->rnm : HIL_GENERIC_NAME;

	ptr->dev.idbus = BUS_HIL;
	ptr->dev.idvendor = SERIO_HIL;
	ptr->dev.idproduct = 0x0001; /* TODO: get from ptr->rsc */
	ptr->dev.idversion = 0x0100; /* TODO: get from ptr->rsc */

	input_register_device(&ptr->dev);
	printk(KERN_INFO "input%d: %s on hil%d\n",
		ptr->dev.number, 
		(btntype == BTN_MOUSE) ? "HIL mouse":"HIL tablet or touchpad",
		0);

	return;
 bail1:
	serio_close(serio);
 bail0:
	kfree(ptr);
	return;
}


struct serio_dev hil_ptr_serio_dev = {
	connect: hil_ptr_connect,
	disconnect: hil_ptr_disconnect,
	interrupt: hil_ptr_interrupt
};

static int __init hil_ptr_init(void)
{
	serio_register_device(&hil_ptr_serio_dev);
        return 0;
}
                
static void __exit hil_ptr_exit(void)
{
	serio_unregister_device(&hil_ptr_serio_dev);
}
                        
module_init(hil_ptr_init);
module_exit(hil_ptr_exit);
