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
 * HP i8042-based System Device Controller driver.
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
 * System Device Controller Microprocessor Firmware Theory of Operation
 *      for Part Number 1820-4784 Revision B.  Dwg No. A-1820-4784-2
 * Helge Deller's original hilkbd.c port for PA-RISC.
 *
 *
 * Driver theory of operation:
 *
 * hp_sdc_put does all writing to the SDC.  ISR can run on a different 
 * CPU than hp_sdc_put, but only one CPU runs hp_sdc_put at a time 
 * (it cannot really benefit from SMP anyway.)  A tasket fit this perfectly.
 *
 * All data coming back from the SDC is sent via interrupt and can be read 
 * fully in the ISR, so there are no latency/throughput problems there.  
 * The problem is with output, due to the slow clock speed of the SDC 
 * compared to the CPU.  This should not be too horrible most of the time, 
 * but if used with HIL devices that support the multibyte transfer command, 
 * keeping outbound throughput flowing at the 6500KBps that the HIL is 
 * capable of is more than can be done at HZ=100.
 *
 * Busy polling for IBF clear wastes CPU cycles and bus cycles.  hp_sdc.ibf 
 * is set to 0 when the IBF flag in the status register has cleared.  ISR 
 * may do this, and may also access the parts of queued transactions related 
 * to reading data back from the SDC, but otherwise will not touch the 
 * hp_sdc state. Whenever a register is written hp_sdc.ibf is set to 1.
 *
 * The i8042 write index and the values in the 4-byte input buffer
 * starting at 0x70 are kept track of in hp_sdc.wi, and .r7[], respectively,
 * to minimize the amount of IO needed to the SDC.  However these values 
 * do not need to be locked since they are only ever accessed by hp_sdc_put.
 *
 * A timer task schedules the tasklet once per second just to make
 * sure it doesn't freeze up and to allow for bad reads to time out.
 */

#include <linux/hp_sdc.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/hil.h>
#include <asm/io.h>
#include <asm/system.h>

#include <asm/gsc.h>

#define PREFIX "HP SDC: "

MODULE_AUTHOR("Brian S. Julin <bri@calyx.com>");
MODULE_DESCRIPTION("HP i8042-based SDC Driver");
MODULE_LICENSE("Dual BSD/GPL");

EXPORT_SYMBOL(hp_sdc_request_timer_irq);
EXPORT_SYMBOL(hp_sdc_request_hil_irq);
EXPORT_SYMBOL(hp_sdc_request_cooked_irq);

EXPORT_SYMBOL(hp_sdc_release_timer_irq);
EXPORT_SYMBOL(hp_sdc_release_hil_irq);
EXPORT_SYMBOL(hp_sdc_release_cooked_irq);

EXPORT_SYMBOL(hp_sdc_enqueue_transaction);
EXPORT_SYMBOL(hp_sdc_dequeue_transaction);

static hp_i8042_sdc	hp_sdc;	/* All driver state is kept in here. */

/*************** primitives for use in any context *********************/
static inline uint8_t hp_sdc_status_in8 (void) {
	uint8_t status;
	unsigned long flags;

	write_lock_irqsave(&hp_sdc.ibf_lock, flags);
	status = gsc_readb(hp_sdc.status_io);
	if (!(status & HP_SDC_STATUS_IBF)) hp_sdc.ibf = 0;
	write_unlock_irqrestore(&hp_sdc.ibf_lock, flags);

	return status;
}

static inline uint8_t hp_sdc_data_in8 (void) {
	return gsc_readb(hp_sdc.data_io); 
}

static inline void hp_sdc_status_out8 (uint8_t val) {
	unsigned long flags;

	write_lock_irqsave(&hp_sdc.ibf_lock, flags);
	hp_sdc.ibf = 1;
	if ((val & 0xf0) == 0xe0) hp_sdc.wi = 0xff;
	gsc_writeb(val, hp_sdc.status_io);
	write_unlock_irqrestore(&hp_sdc.ibf_lock, flags);
}

static inline void hp_sdc_data_out8 (uint8_t val) {
	unsigned long flags;

	write_lock_irqsave(&hp_sdc.ibf_lock, flags);
	hp_sdc.ibf = 1;
	gsc_writeb(val, hp_sdc.data_io);
	write_unlock_irqrestore(&hp_sdc.ibf_lock, flags);
}

/*	Care must be taken to only invoke hp_sdc_spin_ibf when 
 *	absolutely needed, or in rarely invoked subroutines.  
 *	Not only does it waste CPU cycles, it also wastes bus cycles. 
 */
static inline void hp_sdc_spin_ibf(void) {
	unsigned long flags;
	rwlock_t *lock;

	lock = &hp_sdc.ibf_lock;

	read_lock_irqsave(lock, flags);
	if (!hp_sdc.ibf) {
		read_unlock_irqrestore(lock, flags);
		return;
	}
	read_unlock(lock);
	write_lock(lock);
	while (gsc_readb(hp_sdc.status_io) & HP_SDC_STATUS_IBF) {};
	hp_sdc.ibf = 0;
	write_unlock_irqrestore(lock, flags);
}


/************************ Interrupt context functions ************************/
static void hp_sdc_take (int irq, void *dev_id, uint8_t status, uint8_t data) {
	hp_sdc_transaction *curr;

	read_lock(&hp_sdc.rtq_lock);
	if (hp_sdc.rcurr < 0) {
	  	read_unlock(&hp_sdc.rtq_lock);
		return;
	}
	curr = hp_sdc.tq[hp_sdc.rcurr];
	read_unlock(&hp_sdc.rtq_lock);

	curr->seq[curr->idx++] = status;
	curr->seq[curr->idx++] = data;
	hp_sdc.rqty -= 2;
	do_gettimeofday(&hp_sdc.rtv);

	if (hp_sdc.rqty <= 0) {
		/* All data has been gathered. */
		if(curr->seq[curr->actidx] & HP_SDC_ACT_SEMAPHORE) {
			if (curr->act.semaphore) up(curr->act.semaphore);
		}
		if(curr->seq[curr->actidx] & HP_SDC_ACT_CALLBACK) {
			if (curr->act.irqhook)
				curr->act.irqhook(irq, dev_id, status, data);
		}
		curr->actidx = curr->idx;
		curr->idx++;
		/* Return control of this transaction */
		write_lock(&hp_sdc.rtq_lock);
		hp_sdc.rcurr = -1; 
		hp_sdc.rqty = 0;
		write_unlock(&hp_sdc.rtq_lock);
		tasklet_schedule(&hp_sdc.task);
	}
}

static void hp_sdc_isr(int irq, void *dev_id, struct pt_regs * regs) {
	uint8_t status, data;

	status = hp_sdc_status_in8();
	/* Read data unconditionally to advance i8042. */
	data =   hp_sdc_data_in8();

	/* For now we are ignoring these until we get the SDC to behave. */
	if (((status & 0xf1) == 0x51) && data == 0x82) {
	  return;
	}

	switch(status & HP_SDC_STATUS_IRQMASK) {
	      case 0: /* This case is not documented. */
		break;
	      case HP_SDC_STATUS_USERTIMER:
	      case HP_SDC_STATUS_PERIODIC:
	      case HP_SDC_STATUS_TIMER:
		read_lock(&hp_sdc.hook_lock);
	      	if (hp_sdc.timer != NULL)
			hp_sdc.timer(irq, dev_id, status, data);
		read_unlock(&hp_sdc.hook_lock);
		break;
	      case HP_SDC_STATUS_REG:
		hp_sdc_take(irq, dev_id, status, data);
		break;
	      case HP_SDC_STATUS_HILCMD:
	      case HP_SDC_STATUS_HILDATA:
		read_lock(&hp_sdc.hook_lock);
		if (hp_sdc.hil != NULL)
			hp_sdc.hil(irq, dev_id, status, data);
		read_unlock(&hp_sdc.hook_lock);
		break;
	      case HP_SDC_STATUS_PUP:
		read_lock(&hp_sdc.hook_lock);
		if (hp_sdc.pup != NULL)
			hp_sdc.pup(irq, dev_id, status, data);
		else printk(KERN_INFO PREFIX "HP SDC reports successful PUP.\n");
		read_unlock(&hp_sdc.hook_lock);
		break;
	      default:
		read_lock(&hp_sdc.hook_lock);
		if (hp_sdc.cooked != NULL)
			hp_sdc.cooked(irq, dev_id, status, data);
		read_unlock(&hp_sdc.hook_lock);
		break;
	}
}


static void hp_sdc_nmisr(int irq, void *dev_id, struct pt_regs * regs) {
	int status;
	
	status = hp_sdc_status_in8();
	printk(KERN_WARNING PREFIX "NMI !\n");

}


/***************** Kernel (tasklet) context functions ****************/

unsigned long hp_sdc_put(void);

static void hp_sdc_tasklet(unsigned long foo) {

	write_lock_irq(&hp_sdc.rtq_lock);
	if (hp_sdc.rcurr >= 0) {
		struct timeval tv;
		do_gettimeofday(&tv);
		if (tv.tv_sec > hp_sdc.rtv.tv_sec) tv.tv_usec += 1000000;
		if (tv.tv_usec - hp_sdc.rtv.tv_usec > HP_SDC_MAX_REG_DELAY) {
			hp_sdc_transaction *curr;
			uint8_t tmp;

			curr = hp_sdc.tq[hp_sdc.rcurr];
			/* If this turns out to be a normal failure mode
			 * we'll need to figure out a way to communicate
			 * it back to the application. and be less verbose.
			 */
			printk(KERN_WARNING PREFIX "read timeout (%ius)!\n",
			       tv.tv_usec - hp_sdc.rtv.tv_usec);
			curr->idx += hp_sdc.rqty;
			hp_sdc.rqty = 0;
			tmp = curr->seq[curr->actidx];
			curr->seq[curr->actidx] |= HP_SDC_ACT_DEAD;
			if(tmp & HP_SDC_ACT_SEMAPHORE) {
				if (curr->act.semaphore) 
					up(curr->act.semaphore);
			}
			if(tmp & HP_SDC_ACT_CALLBACK) {
				/* Note this means that irqhooks may be called
				 * in tasklet/bh context.
				 */
				if (curr->act.irqhook) 
					curr->act.irqhook(0, 0, 0, 0);
			}
			curr->actidx = curr->idx;
			curr->idx++;
			hp_sdc.rcurr = -1; 
		}
	}
	write_unlock_irq(&hp_sdc.rtq_lock);
	hp_sdc_put();
}

unsigned long hp_sdc_put(void) {
	hp_sdc_transaction *curr;
	uint8_t act;
	int idx, curridx;

	int limit = 0;

	write_lock(&hp_sdc.lock);

	/* If i8042 buffers are full, we cannot do anything that
	   requires output, so we skip to the administrativa. */
	if (hp_sdc.ibf) {
		hp_sdc_status_in8();
		if (hp_sdc.ibf) goto finish;
	}

 anew:
	/* See if we are in the middle of a sequence. */
	if (hp_sdc.wcurr < 0) hp_sdc.wcurr = 0;
	read_lock_irq(&hp_sdc.rtq_lock);
	if (hp_sdc.rcurr == hp_sdc.wcurr) hp_sdc.wcurr++;
	read_unlock_irq(&hp_sdc.rtq_lock);
	if (hp_sdc.wcurr >= HP_SDC_QUEUE_LEN) hp_sdc.wcurr = 0;
	curridx = hp_sdc.wcurr;

	if (hp_sdc.tq[curridx] != NULL) goto start;

	while (++curridx != hp_sdc.wcurr) {
		if (curridx >= HP_SDC_QUEUE_LEN) {
			curridx = -1; /* Wrap to top */
			continue;
		}
		read_lock_irq(&hp_sdc.rtq_lock);
		if (hp_sdc.rcurr == curridx) {
			read_unlock_irq(&hp_sdc.rtq_lock);
			continue;
		}
		read_unlock_irq(&hp_sdc.rtq_lock);
		if (hp_sdc.tq[curridx] != NULL) break; /* Found one. */
	}
	if (curridx == hp_sdc.wcurr) { /* There's nothing queued to do. */
		curridx = -1;
	}
	hp_sdc.wcurr = curridx;

 start:

	/* Check to see if the interrupt mask needs to be set. */
	if (hp_sdc.set_im) {
		hp_sdc_status_out8(hp_sdc.im | HP_SDC_CMD_SET_IM);
		hp_sdc.set_im = 0;
		goto finish;
	}

	if (hp_sdc.wcurr == -1) goto done;

	curr = hp_sdc.tq[curridx];
	idx = curr->actidx;

	if (curr->actidx >= curr->endidx) {
		hp_sdc.tq[curridx] = NULL;
		/* Interleave outbound data between the transactions. */
		hp_sdc.wcurr++;
		if (hp_sdc.wcurr >= HP_SDC_QUEUE_LEN) hp_sdc.wcurr = 0;
		goto finish;	
	}

	act = curr->seq[idx];
	idx++;

	if (curr->idx >= curr->endidx) {
		if (act & HP_SDC_ACT_DEALLOC) kfree(curr);
		hp_sdc.tq[curridx] = NULL;
		/* Interleave outbound data between the transactions. */
		hp_sdc.wcurr++;
		if (hp_sdc.wcurr >= HP_SDC_QUEUE_LEN) hp_sdc.wcurr = 0;
		goto finish;	
	}

	while (act & HP_SDC_ACT_PRECMD) {
		if (curr->idx != idx) {
			idx++;
			act &= ~HP_SDC_ACT_PRECMD;
			break;
		}
		hp_sdc_status_out8(curr->seq[idx]);
		curr->idx++;
		/* act finished? */
		if ((act & HP_SDC_ACT_DURING) == HP_SDC_ACT_PRECMD)
		  goto actdone;
		/* skip quantity field if data-out sequence follows. */
		if (act & HP_SDC_ACT_DATAOUT) curr->idx++;
		goto finish;
	}
	if (act & HP_SDC_ACT_DATAOUT) {
		int qty;

		qty = curr->seq[idx];
		idx++;
		if (curr->idx - idx < qty) {
			hp_sdc_data_out8(curr->seq[curr->idx]);
			curr->idx++;
			/* act finished? */
			if ((curr->idx - idx >= qty) && 
			    ((act & HP_SDC_ACT_DURING) == HP_SDC_ACT_DATAOUT))
				goto actdone;
			goto finish;
		}
		idx += qty;
		act &= ~HP_SDC_ACT_DATAOUT;
	}
	else while (act & HP_SDC_ACT_DATAREG) {
		int mask;
		uint8_t w7[4];

		mask = curr->seq[idx];
		if (idx != curr->idx) {
			idx++;
			idx += !!(mask & 1);
			idx += !!(mask & 2);
			idx += !!(mask & 4);
			idx += !!(mask & 8);
			act &= ~HP_SDC_ACT_DATAREG;
			break;
		}
		
		w7[0] = (mask & 1) ? curr->seq[++idx] : hp_sdc.r7[0];
		w7[1] = (mask & 2) ? curr->seq[++idx] : hp_sdc.r7[1];
		w7[2] = (mask & 4) ? curr->seq[++idx] : hp_sdc.r7[2];
		w7[3] = (mask & 8) ? curr->seq[++idx] : hp_sdc.r7[3];
		
		if (hp_sdc.wi > 0x73 || hp_sdc.wi < 0x70 ||
		        w7[hp_sdc.wi-0x70] == hp_sdc.r7[hp_sdc.wi-0x70]) {
			int i = 0;

			/* Need to point the write index register */	
			while ((i < 4) && w7[i] == hp_sdc.r7[i]) i++;
			if (i < 4) {
				hp_sdc_status_out8(HP_SDC_CMD_SET_D0 + i);
				hp_sdc.wi = 0x70 + i;
				goto finish;
			}
			idx++;
			if ((act & HP_SDC_ACT_DURING) == HP_SDC_ACT_DATAREG)
				goto actdone;
			curr->idx = idx;
			act &= ~HP_SDC_ACT_DATAREG;
			break;
		}

		hp_sdc_data_out8(w7[hp_sdc.wi - 0x70]);
		hp_sdc.r7[hp_sdc.wi - 0x70] = w7[hp_sdc.wi - 0x70];
		hp_sdc.wi++; /* write index register autoincrements */
		{
			int i = 0;

			while ((i < 4) && w7[i] == hp_sdc.r7[i]) i++;
			if (i >= 4) {
				curr->idx = idx + 1;
				if ((act & HP_SDC_ACT_DURING) == 
				    HP_SDC_ACT_DATAREG)
				        goto actdone;
			}
		}
		goto finish;
	}
	/* We don't go any further in the command if there is a pending read,
	   because we don't want interleaved results. */
	read_lock_irq(&hp_sdc.rtq_lock);
	if (hp_sdc.rcurr >= 0) {
		read_unlock_irq(&hp_sdc.rtq_lock);
		goto finish;
	}
	read_unlock_irq(&hp_sdc.rtq_lock);


	if (act & HP_SDC_ACT_POSTCMD) {
	  	uint8_t postcmd;

		/* curr->idx should == idx at this point. */
		postcmd = curr->seq[idx];
		curr->idx++;
		if (act & HP_SDC_ACT_DATAIN) {

			/* Start a new read */
	  		hp_sdc.rqty = curr->seq[curr->idx];
			do_gettimeofday(&hp_sdc.rtv);
			curr->idx++;
			/* Still need to lock here in case of spurious irq. */
			write_lock_irq(&hp_sdc.rtq_lock);
			hp_sdc.rcurr = curridx; 
			write_unlock_irq(&hp_sdc.rtq_lock);
			hp_sdc_status_out8(postcmd);
			goto finish;
		}
		hp_sdc_status_out8(postcmd);
		goto actdone;
	}

actdone:
	if (act & HP_SDC_ACT_SEMAPHORE) {
		up(curr->act.semaphore);
	}
	else if (act & HP_SDC_ACT_CALLBACK) {
		curr->act.irqhook(0,0,0,0);
	}
	if (curr->idx >= curr->endidx) { /* This transaction is over. */
		if (act & HP_SDC_ACT_DEALLOC) kfree(curr);
		hp_sdc.tq[curridx] = NULL;
	}
	else {
		curr->actidx = idx + 1;
		curr->idx = idx + 2;
	}
	/* Interleave outbound data between the transactions. */
	hp_sdc.wcurr++;
	if (hp_sdc.wcurr >= HP_SDC_QUEUE_LEN) hp_sdc.wcurr = 0;

 finish:
	/* If by some quirk IBF has cleared and our ISR has run to 
	   see that that has happened, do it all again. */
	if (!hp_sdc.ibf && limit++ < 20) goto anew;

 done:
	if (hp_sdc.wcurr >= 0) tasklet_schedule(&hp_sdc.task);
	write_unlock(&hp_sdc.lock);
	return 0;
}

/******* Functions called in either user or kernel context ****/
int hp_sdc_enqueue_transaction(hp_sdc_transaction *this) {
	int i;

	if (this == NULL) {
		tasklet_schedule(&hp_sdc.task);
		return -EINVAL;
	};

	write_lock_bh(&hp_sdc.lock);

	/* Can't have same transaction on queue twice */
	for (i=0; i < HP_SDC_QUEUE_LEN; i++)
		if (hp_sdc.tq[i] == this) goto fail;

	this->actidx = 0;
	this->idx = 1;

	/* Search for empty slot */
	for (i=0; i < HP_SDC_QUEUE_LEN; i++) {
		if (hp_sdc.tq[i] == NULL) {
			hp_sdc.tq[i] = this;
			tasklet_schedule(&hp_sdc.task);
			write_unlock_bh(&hp_sdc.lock);
			return 0;
		}
	}
	write_unlock_bh(&hp_sdc.lock);
	printk(KERN_WARNING PREFIX "No free slot to add transaction.\n");
	return -EBUSY;

 fail:
	write_unlock_bh(&hp_sdc.lock);
	printk(KERN_WARNING PREFIX "Transaction add failed: transaction already queued?\n");
	return -EINVAL;
}

int hp_sdc_dequeue_transaction(hp_sdc_transaction *this) {
	int i;

	write_lock_bh(&hp_sdc.lock);

	/* TODO: don't remove it if it's not done. */

	for (i=0; i < HP_SDC_QUEUE_LEN; i++)
		if (hp_sdc.tq[i] == this) hp_sdc.tq[i] = NULL;

	write_unlock_bh(&hp_sdc.lock);
	return 0;
}



/********************** User context functions **************************/
int hp_sdc_request_timer_irq(hp_sdc_irqhook *callback) {

	MOD_INC_USE_COUNT;
	if (callback == NULL || hp_sdc.dev == NULL) {
		MOD_DEC_USE_COUNT;
		return -EINVAL;
	}
	write_lock_irq(&hp_sdc.hook_lock);
	if (hp_sdc.timer != NULL) {
		write_unlock_irq(&hp_sdc.hook_lock);
		MOD_DEC_USE_COUNT;
		return -EBUSY;
	}

	hp_sdc.timer = callback;
	/* Enable interrupts from the timers */
	hp_sdc.im &= ~HP_SDC_IM_FH;
        hp_sdc.im &= ~HP_SDC_IM_PT;
	hp_sdc.im &= ~HP_SDC_IM_TIMERS;
	hp_sdc.set_im = 1;
	write_unlock_irq(&hp_sdc.hook_lock);

	tasklet_schedule(&hp_sdc.task);

	return 0;
}

int hp_sdc_request_hil_irq(hp_sdc_irqhook *callback) {

	MOD_INC_USE_COUNT;
	if (callback == NULL || hp_sdc.dev == NULL) {
		MOD_DEC_USE_COUNT;
		return -EINVAL;
	}
	write_lock_irq(&hp_sdc.hook_lock);
	if (hp_sdc.hil != NULL) {
		write_unlock_irq(&hp_sdc.hook_lock);
		MOD_DEC_USE_COUNT;
		return -EBUSY;
	}

	hp_sdc.hil = callback;
	hp_sdc.im &= ~HP_SDC_IM_HIL;
	hp_sdc.set_im = 1;
	write_unlock_irq(&hp_sdc.hook_lock);

	tasklet_schedule(&hp_sdc.task);

	return 0;
}

int hp_sdc_request_cooked_irq(hp_sdc_irqhook *callback) {

	MOD_INC_USE_COUNT;
	if (callback == NULL || hp_sdc.dev == NULL) {
		MOD_DEC_USE_COUNT;
		return -EINVAL;
	}
	write_lock_irq(&hp_sdc.hook_lock);
	if (hp_sdc.cooked != NULL) {
		write_unlock_irq(&hp_sdc.hook_lock);
		MOD_DEC_USE_COUNT;
		return -EBUSY;
	}

	/* Enable interrupts from the HIL MLC */
	hp_sdc.cooked = callback;
	hp_sdc.im &= ~HP_SDC_IM_HIL;
	hp_sdc.set_im = 1;
	write_unlock_irq(&hp_sdc.hook_lock);

	tasklet_schedule(&hp_sdc.task);

	return 0;
}

int hp_sdc_release_timer_irq(hp_sdc_irqhook *callback) {


	write_lock_irq(&hp_sdc.hook_lock);
	if ((callback != hp_sdc.timer) ||
	    (hp_sdc.timer == NULL)) {
		write_unlock_irq(&hp_sdc.hook_lock);
		return -EINVAL;
	}

	/* Disable interrupts from the timers */
	hp_sdc.timer = NULL;
	hp_sdc.im |= HP_SDC_IM_TIMERS;
	hp_sdc.im |= HP_SDC_IM_FH;
	hp_sdc.im |= HP_SDC_IM_PT;
	hp_sdc.set_im = 1;
	write_unlock_irq(&hp_sdc.hook_lock);
	tasklet_schedule(&hp_sdc.task);

	MOD_DEC_USE_COUNT;
	return 0;
}

int hp_sdc_release_hil_irq(hp_sdc_irqhook *callback) {

	write_lock_irq(&hp_sdc.hook_lock);
	if ((callback != hp_sdc.hil) ||
	    (hp_sdc.hil == NULL)) {
		write_unlock_irq(&hp_sdc.hook_lock);
		return -EINVAL;
	}

	hp_sdc.hil = NULL;
	/* Disable interrupts from HIL only if there is no cooked driver. */
	if(hp_sdc.cooked == NULL) {
		hp_sdc.im |= HP_SDC_IM_HIL;
		hp_sdc.set_im = 1;
	}
	write_unlock_irq(&hp_sdc.hook_lock);
	tasklet_schedule(&hp_sdc.task);

	MOD_DEC_USE_COUNT;
	return 0;
}

int hp_sdc_release_cooked_irq(hp_sdc_irqhook *callback) {

	write_lock_irq(&hp_sdc.hook_lock);
	if ((callback != hp_sdc.cooked) ||
	    (hp_sdc.cooked == NULL)) {
		write_unlock_irq(&hp_sdc.hook_lock);
		return -EINVAL;
	}

	hp_sdc.cooked = NULL;
	/* Disable interrupts from HIL only if there is no raw HIL driver. */
	if(hp_sdc.hil == NULL) {
		hp_sdc.im |= HP_SDC_IM_HIL;
		hp_sdc.set_im = 1;
	}
	write_unlock_irq(&hp_sdc.hook_lock);
	tasklet_schedule(&hp_sdc.task);

	MOD_DEC_USE_COUNT;
	return 0;
}

/************************* Keepalive timer task *********************/

void hp_sdc_kicker (unsigned long data) {
	tasklet_schedule(&hp_sdc.task);
	/* Re-insert the periodic task. */
	mod_timer(&hp_sdc.kicker, jiffies + HZ);
}

/************************** Module Initialization ***************************/

static struct parisc_device_id hp_sdc_tbl[] = {
	{
		hw_type:	HPHW_FIO, 
		hversion_rev:	HVERSION_REV_ANY_ID,
		hversion:	HVERSION_ANY_ID,
		sversion:	0x73, 
	 },
	{ 0, }
};

MODULE_DEVICE_TABLE(parisc, hp_sdc_tbl);

static int __init hp_sdc_init(struct parisc_device *d);

static struct parisc_driver hp_sdc_driver = {
	name:		"HP SDC",
	id_table:	hp_sdc_tbl,
	probe:		hp_sdc_init,
};

static int __init hp_sdc_init(struct parisc_device *d)
{
	int i;
	char *errstr = NULL;
	hp_sdc_transaction t_sync;
	uint8_t ts_sync[6];
	struct semaphore s_sync;


	errstr = "foo\n";

	if (!d) return 1;
	if (hp_sdc.dev != NULL) return 1;	/* We only expect one SDC */

	hp_sdc.dev = d;
	hp_sdc.irq		= d->irq;
	/* TODO: Is NMI == IRQ - 1 all cases, or is there a way to query? */
	hp_sdc.nmi		= d->irq - 1;
	hp_sdc.base_io		= (unsigned long) d->hpa;
	hp_sdc.data_io		= (unsigned long) d->hpa + 0x800;
	hp_sdc.status_io	= (unsigned long) d->hpa + 0x801;

  	hp_sdc.lock		= RW_LOCK_UNLOCKED;
  	hp_sdc.ibf_lock		= RW_LOCK_UNLOCKED;
  	hp_sdc.rtq_lock		= RW_LOCK_UNLOCKED;
  	hp_sdc.hook_lock	= RW_LOCK_UNLOCKED;

	hp_sdc.timer		= NULL;
	hp_sdc.hil		= NULL;
	hp_sdc.pup		= NULL;
	hp_sdc.cooked		= NULL;
	hp_sdc.im		= HP_SDC_IM_MASK;  /* Mask maskable irqs */
	hp_sdc.set_im		= 1;
	hp_sdc.wi		= 0xff;
	hp_sdc.r7[0]		= 0xff;
	hp_sdc.r7[1]		= 0xff;
	hp_sdc.r7[2]		= 0xff;
	hp_sdc.r7[3]		= 0xff;
	hp_sdc.ibf		= 1;

	for (i = 0; i < HP_SDC_QUEUE_LEN; i++) hp_sdc.tq[i] = NULL;
	hp_sdc.wcurr		= -1;
        hp_sdc.rcurr		= -1;
	hp_sdc.rqty		= 0;

	hp_sdc.dev_err = -ENODEV;

	errstr = "IO not found for";
	if (!hp_sdc.base_io) goto err0;

	errstr = "IRQ not found for";
	if (!hp_sdc.irq) goto err0;

	hp_sdc.dev_err = -EBUSY;

	errstr = "IO not available for";
        if (request_region(hp_sdc.data_io, 2, hp_sdc_driver.name)) goto err0;

	errstr = "IRQ not available for";
        if(request_irq(hp_sdc.irq, &hp_sdc_isr, 0, hp_sdc_driver.name, 
		       (void *) hp_sdc.base_io)) goto err1;

	errstr = "NMI not available for";
        if (request_irq(hp_sdc.nmi, &hp_sdc_nmisr, 0, "HP SDC NMI", 
			(void*)d->hpa)) goto err2;

	printk(KERN_INFO PREFIX "HP SDC at 0x%p, IRQ %d (NMI IRQ %d)\n", 
	       (void *)hp_sdc.base_io, hp_sdc.irq, hp_sdc.nmi);

	hp_sdc_status_in8();
	hp_sdc_data_in8();

	tasklet_init(&hp_sdc.task, hp_sdc_tasklet, 0);

	/* Sync the output buffer registers, thus scheduling hp_sdc_tasklet. */
	t_sync.actidx	= 0;
	t_sync.idx	= 1;
	t_sync.endidx	= 6;
	t_sync.seq	= ts_sync;
	ts_sync[0]	= HP_SDC_ACT_DATAREG | HP_SDC_ACT_SEMAPHORE;
	ts_sync[1]	= 0x0f;
	ts_sync[2] = ts_sync[3]	= ts_sync[4] = ts_sync[5] = 0;
	t_sync.act.semaphore = &s_sync;
	init_MUTEX_LOCKED(&s_sync);
	hp_sdc_enqueue_transaction(&t_sync);
	down(&s_sync); /* Wait for t_sync to complete */

	/* Create the keepalive task */
	init_timer(&hp_sdc.kicker);
	hp_sdc.kicker.expires = jiffies + HZ;
	hp_sdc.kicker.function = &hp_sdc_kicker;
	add_timer(&hp_sdc.kicker);

	hp_sdc.dev_err = 0;
	return 0;
 err2:
	free_irq(hp_sdc.irq, NULL);
 err1:
	release_region(hp_sdc.data_io, 2);
 err0:
	printk(KERN_WARNING PREFIX ": %s SDC IO=0x%p IRQ=0x%x NMI=0x%x\n", 
		errstr, (void *)hp_sdc.base_io, hp_sdc.irq, hp_sdc.nmi);
	hp_sdc.dev = NULL;
	return hp_sdc.dev_err;
}

static void __exit hp_sdc_exit(void)
{
	write_lock_irq(&hp_sdc.lock);

	/* Turn off all maskable "sub-function" irq's. */
	hp_sdc_spin_ibf();
	gsc_writeb(HP_SDC_CMD_SET_IM | HP_SDC_IM_MASK, hp_sdc.status_io);

	/* Wait until we know this has been processed by the i8042 */
	hp_sdc_spin_ibf();

	free_irq(hp_sdc.nmi, NULL);
	free_irq(hp_sdc.irq, NULL);
	write_unlock_irq(&hp_sdc.lock);

	del_timer(&hp_sdc.kicker);

	tasklet_kill(&hp_sdc.task);

/*        release_region(hp_sdc.data_io, 2); */

	if (unregister_parisc_driver(&hp_sdc_driver)) 
		printk(KERN_WARNING PREFIX "Error unregistering HP SDC");
}

static int __init hp_sdc_register(void)
{
	hp_sdc.dev = NULL;
	hp_sdc.dev_err = 0;
	hp_sdc_transaction tq_init;
	uint8_t tq_init_seq[5];
	struct semaphore tq_init_sem;

	if (register_parisc_driver(&hp_sdc_driver)) {
		printk(KERN_WARNING PREFIX "Error registering SDC with system bus tree.\n");
		return -ENODEV;
	}
	if (hp_sdc.dev == NULL) {
		printk(KERN_WARNING PREFIX "No SDC found.\n");
		return hp_sdc.dev_err;
	}

	init_MUTEX_LOCKED(&tq_init_sem);

	tq_init.actidx		= 0;
	tq_init.idx		= 1;
	tq_init.endidx		= 5;
	tq_init.seq		= tq_init_seq;
	tq_init.act.semaphore	= &tq_init_sem;

	tq_init_seq[0] = 
	  HP_SDC_ACT_POSTCMD | HP_SDC_ACT_DATAIN | HP_SDC_ACT_SEMAPHORE;
	tq_init_seq[1] = HP_SDC_CMD_READ_KCC;
	tq_init_seq[2] = 1;
	tq_init_seq[3] = 0;
	tq_init_seq[4] = 0;

	hp_sdc_enqueue_transaction(&tq_init);

	down(&tq_init_sem);
	up(&tq_init_sem);

	if ((tq_init_seq[0] & HP_SDC_ACT_DEAD) == HP_SDC_ACT_DEAD) {
		printk(KERN_WARNING PREFIX "Error reading config byte.\n");
		hp_sdc_exit();
		return -ENODEV;
	}
	hp_sdc.r11 = tq_init_seq[4];
	if (hp_sdc.r11 & HP_SDC_CFG_NEW) {
		char *str;
		printk(KERN_INFO PREFIX "New style SDC\n");
		tq_init_seq[1] = HP_SDC_CMD_READ_XTD;
		tq_init.actidx		= 0;
		tq_init.idx		= 1;
		down(&tq_init_sem);
		hp_sdc_enqueue_transaction(&tq_init);		
		down(&tq_init_sem);
		up(&tq_init_sem);
		if ((tq_init_seq[0] & HP_SDC_ACT_DEAD) == HP_SDC_ACT_DEAD) {
			printk(KERN_WARNING PREFIX "Error reading extended config byte.\n");
			return -ENODEV;
		}
		hp_sdc.r7e = tq_init_seq[4];
		HP_SDC_XTD_REV_STRINGS(hp_sdc.r7e & HP_SDC_XTD_REV, str)
		printk(KERN_INFO PREFIX "Revision: %s\n", str);
		if (hp_sdc.r7e & HP_SDC_XTD_BEEPER) {
			printk(KERN_INFO PREFIX "TI SN76494 beeper present\n");
		}
		if (hp_sdc.r7e & HP_SDC_XTD_BBRTC) {
			printk(KERN_INFO PREFIX "OKI MSM-58321 BBRTC present\n");
		}
		printk(KERN_INFO PREFIX "Spunking the self test register to force PUP "
		       "on next firmware reset.\n");
		tq_init_seq[0] = HP_SDC_ACT_PRECMD | 
			HP_SDC_ACT_DATAOUT | HP_SDC_ACT_SEMAPHORE;
		tq_init_seq[1] = HP_SDC_CMD_SET_STR;
		tq_init_seq[2] = 1;
		tq_init_seq[3] = 0;
		tq_init.actidx		= 0;
		tq_init.idx		= 1;
		tq_init.endidx		= 4;
		down(&tq_init_sem);
		hp_sdc_enqueue_transaction(&tq_init);		
		down(&tq_init_sem);
		up(&tq_init_sem);
	}
	else {
		printk(KERN_INFO PREFIX "Old style SDC (1820-%s).\n", 
		       (hp_sdc.r11 & HP_SDC_CFG_REV) ? "3300" : "2564/3087");
	}

        return 0;
}

module_init(hp_sdc_register);
module_exit(hp_sdc_exit);

/* Timing notes:  These measurements taken on my 64MHz 7100-LC (715/64) 
 *                                              cycles cycles-adj    time
 * between two consecutive mfctl(16)'s:              4        n/a    63ns
 * hp_sdc_spin_ibf when idle:                      119        115   1.7us
 * gsc_writeb status register:                      83         79   1.2us
 * IBF to clear after sending SET_IM:             6204       6006    93us
 * IBF to clear after sending LOAD_RT:            4467       4352    68us  
 * IBF to clear after sending two LOAD_RTs:      18974      18859   295us
 * READ_T1, read status/data, IRQ, call handler: 35564        n/a   556us
 * cmd to ~IBF READ_T1 2nd time right after:   5158403        n/a    81ms
 * between IRQ received and ~IBF for above:    2578877        n/a    40ms
 *
 * Performance stats after a run of this module configuring HIL and
 * receiving a few mouse events:
 *
 * status in8  282508 cycles 7128 calls
 * status out8   8404 cycles  341 calls
 * data out8     1734 cycles   78 calls
 * isr         174324 cycles  617 calls (includes take)
 * take          1241 cycles    2 calls
 * put        1411504 cycles 6937 calls
 * task       1655209 cycles 6937 calls (includes put)
 *
 */
