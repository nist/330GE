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
#ifndef __HISAX_ISAC_H__
#define __HISAX_ISAC_H__

#include <linux/kernel.h>
#include "fsm.h"
#include "hisax_if.h"

#define TIMER3_VALUE 7000
#define MAX_DFRAME_LEN_L1 300

#define ISAC_IOM1	0

struct isac {
	void *priv;

	u_long flags;
	struct hisax_d_if hisax_d_if;
	struct FsmInst l1m;
	struct FsmTimer timer;
	u_char mocr;
	u_char adf2;
	int    type;

	u_char rcvbuf[MAX_DFRAME_LEN_L1];
	int rcvidx;

	struct sk_buff *tx_skb;
	int tx_cnt;

	u_char (*read_isac)      (struct isac *, u_char);
	void   (*write_isac)     (struct isac *, u_char, u_char);
	void   (*read_isac_fifo) (struct isac *, u_char *, int);
	void   (*write_isac_fifo)(struct isac *, u_char *, int);
};

void isac_init(struct isac *isac);
void isac_d_l2l1(struct hisax_if *hisax_d_if, int pr, void *arg);

void isac_setup(struct isac *isac);
void isac_irq(struct isac *isac);

void isacsx_setup(struct isac *isac);
void isacsx_irq(struct isac *isac);

#endif
