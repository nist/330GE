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
 * These are the public elements of the Linux LAPB module.
 */

#ifndef	LAPB_KERNEL_H
#define	LAPB_KERNEL_H

#define	LAPB_OK			0
#define	LAPB_BADTOKEN		1
#define	LAPB_INVALUE		2
#define	LAPB_CONNECTED		3
#define	LAPB_NOTCONNECTED	4
#define	LAPB_REFUSED		5
#define	LAPB_TIMEDOUT		6
#define	LAPB_NOMEM		7

#define	LAPB_STANDARD		0x00
#define	LAPB_EXTENDED		0x01

#define	LAPB_SLP		0x00
#define	LAPB_MLP		0x02

#define	LAPB_DTE		0x00
#define	LAPB_DCE		0x04

struct lapb_register_struct {
	void (*connect_confirmation)(void *token, int reason);
	void (*connect_indication)(void *token, int reason);
	void (*disconnect_confirmation)(void *token, int reason);
	void (*disconnect_indication)(void *token, int reason);
	int  (*data_indication)(void *token, struct sk_buff *skb);
	void (*data_transmit)(void *token, struct sk_buff *skb);
};

struct lapb_parms_struct {
	unsigned int t1;
	unsigned int t1timer;
	unsigned int t2;
	unsigned int t2timer;
	unsigned int n2;
	unsigned int n2count;
	unsigned int window;
	unsigned int state;
	unsigned int mode;
};

extern int lapb_register(void *token, struct lapb_register_struct *callbacks);
extern int lapb_unregister(void *token);
extern int lapb_getparms(void *token, struct lapb_parms_struct *parms);
extern int lapb_setparms(void *token, struct lapb_parms_struct *parms);
extern int lapb_connect_request(void *token);
extern int lapb_disconnect_request(void *token);
extern int lapb_data_request(void *token, struct sk_buff *skb);
extern int lapb_data_received(void *token, struct sk_buff *skb);

#endif
