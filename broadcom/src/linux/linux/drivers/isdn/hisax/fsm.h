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
/* $Id: fsm.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * Finite state machine
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 *              by Kai Germaschewski <kai.germaschewski@gmx.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef __FSM_H__
#define __FSM_H__

#include <linux/timer.h>

struct FsmInst;

typedef void (* FSMFNPTR)(struct FsmInst *, int, void *);

struct Fsm {
	FSMFNPTR *jumpmatrix;
	int state_count, event_count;
	char **strEvent, **strState;
};

struct FsmInst {
	struct Fsm *fsm;
	int state;
	int debug;
	void *userdata;
	int userint;
	void (*printdebug) (struct FsmInst *, char *, ...);
};

struct FsmNode {
	int state, event;
	void (*routine) (struct FsmInst *, int, void *);
};

struct FsmTimer {
	struct FsmInst *fi;
	struct timer_list tl;
	int event;
	void *arg;
};

int FsmNew(struct Fsm *fsm, struct FsmNode *fnlist, int fncount);
void FsmFree(struct Fsm *fsm);
int FsmEvent(struct FsmInst *fi, int event, void *arg);
void FsmChangeState(struct FsmInst *fi, int newstate);
void FsmInitTimer(struct FsmInst *fi, struct FsmTimer *ft);
int FsmAddTimer(struct FsmTimer *ft, int millisec, int event,
		void *arg, int where);
void FsmRestartTimer(struct FsmTimer *ft, int millisec, int event,
		     void *arg, int where);
void FsmDelTimer(struct FsmTimer *ft, int where);

#endif
