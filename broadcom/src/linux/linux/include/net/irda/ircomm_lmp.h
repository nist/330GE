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
/*********************************************************************
 *                
 * Filename:      ircomm_lmp.h
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Wed Jun  9 10:06:07 1999
 * Modified at:   Fri Aug 13 07:32:32 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999 Dag Brattli, All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *     MA 02111-1307 USA
 *     
 ********************************************************************/

#ifndef IRCOMM_LMP_H
#define IRCOMM_LMP_H

#include <net/irda/ircomm_core.h>
#include <net/irda/ircomm_event.h>

int ircomm_open_lsap(struct ircomm_cb *self);
int  ircomm_lmp_connect_request(struct ircomm_cb *self, 
				struct sk_buff *userdata, 
				struct ircomm_info *info);
int  ircomm_lmp_connect_response(struct ircomm_cb *self, struct sk_buff *skb);
int  ircomm_lmp_disconnect_request(struct ircomm_cb *self, 
				   struct sk_buff *userdata, 
				   struct ircomm_info *info);
int  ircomm_lmp_data_request(struct ircomm_cb *self, struct sk_buff *skb, 
			     int clen);
int  ircomm_lmp_control_request(struct ircomm_cb *self, 
			       struct sk_buff *userdata);
int  ircomm_lmp_data_indication(void *instance, void *sap,
				struct sk_buff *skb);
void ircomm_lmp_connect_confirm(void *instance, void *sap,
				struct qos_info *qos, 
				__u32 max_sdu_size, 
				__u8 max_header_size,
				struct sk_buff *skb);
void ircomm_lmp_connect_indication(void *instance, void *sap,
				   struct qos_info *qos,
				   __u32 max_sdu_size,
				   __u8 max_header_size,
				   struct sk_buff *skb);
void ircomm_lmp_disconnect_indication(void *instance, void *sap, 
				      LM_REASON reason,
				      struct sk_buff *skb);

#endif
