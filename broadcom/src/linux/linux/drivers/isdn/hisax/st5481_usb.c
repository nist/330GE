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
 * Driver for ST5481 USB ISDN modem
 *
 * Author       Frode Isaksen
 * Copyright    2001 by Frode Isaksen      <fisaksen@bewan.com>
 *              2001 by Kai Germaschewski  <kai.germaschewski@gmx.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include <linux/init.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include "st5481.h"

/* ======================================================================
 * control pipe
 */

/*
 * Send the next endpoint 0 request stored in the FIFO.
 * Called either by the completion or by usb_ctrl_msg.
 */
static void usb_next_ctrl_msg(struct urb *urb,
			      struct st5481_adapter *adapter)
{
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	int r_index;

	if (test_and_set_bit(0, &ctrl->busy)) {
		return;
	}

	if ((r_index = fifo_remove(&ctrl->msg_fifo.f)) < 0) {
		test_and_clear_bit(0,&ctrl->busy);
		return;
	} 
	urb->setup_packet = 
		(unsigned char *)&ctrl->msg_fifo.data[r_index];
	
	DBG(1,"request=0x%02x,value=0x%04x,index=%x",
	    ((struct ctrl_msg *)urb->setup_packet)->dr.bRequest,
	    ((struct ctrl_msg *)urb->setup_packet)->dr.wValue,
	    ((struct ctrl_msg *)urb->setup_packet)->dr.wIndex);

	// Prepare the URB
	urb->dev = adapter->usb_dev;

	SUBMIT_URB(urb);
}

/*
 * Asynchronous endpoint 0 request (async version of usb_control_msg).
 * The request will be queued up in a FIFO if the endpoint is busy.
 */
void usb_ctrl_msg(struct st5481_adapter *adapter,
		  u8 request, u8 requesttype, u16 value, u16 index,
		  ctrl_complete_t complete, void *context)
{
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	int w_index;
	struct ctrl_msg *ctrl_msg;
	
	if ((w_index = fifo_add(&ctrl->msg_fifo.f)) < 0) {
		WARN("control msg FIFO full");
		return;
	}
	ctrl_msg = &ctrl->msg_fifo.data[w_index]; 
   
	ctrl_msg->dr.bRequestType = requesttype;
	ctrl_msg->dr.bRequest = request;
	ctrl_msg->dr.wValue = cpu_to_le16p(&value);
	ctrl_msg->dr.wIndex = cpu_to_le16p(&index);
	ctrl_msg->dr.wLength = 0;
	ctrl_msg->complete = complete;
	ctrl_msg->context = context;

	usb_next_ctrl_msg(ctrl->urb, adapter);
}

/*
 * Asynchronous endpoint 0 device request.
 */
void st5481_usb_device_ctrl_msg(struct st5481_adapter *adapter,
			 u8 request, u16 value,
			 ctrl_complete_t complete, void *context)
{
	usb_ctrl_msg(adapter, request, 
		     USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE, 
		     value, 0, complete, context);
}

/*
 * Asynchronous pipe reset (async version of usb_clear_halt).
 */
void st5481_usb_pipe_reset(struct st5481_adapter *adapter,
		    u_char pipe,
		    ctrl_complete_t complete, void *context)
{
	DBG(1,"pipe=%02x",pipe);

	usb_ctrl_msg(adapter,
		     USB_REQ_CLEAR_FEATURE, USB_DIR_OUT | USB_RECIP_ENDPOINT,
		     0, pipe, complete, context);
}


/*
  Physical level functions
*/

void st5481_ph_command(struct st5481_adapter *adapter, unsigned int command)
{
	DBG(8,"command=%s", ST5481_CMD_string(command));

	st5481_usb_device_ctrl_msg(adapter, TXCI, command, NULL, NULL);
}

/*
 * The request on endpoint 0 has completed.
 * Call the user provided completion routine and try
 * to send the next request.
 */
static void usb_ctrl_complete(struct urb *urb)
{
	struct st5481_adapter *adapter = urb->context;
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	struct ctrl_msg *ctrl_msg;
	
	if (urb->status < 0) {
		if (urb->status != USB_ST_URB_KILLED) {
			WARN("urb status %d",urb->status);
		} else {
			DBG(1,"urb killed");
			return; // Give up
		}
	}

	ctrl_msg = (struct ctrl_msg *)urb->setup_packet;
	
	if (ctrl_msg->dr.bRequest == USB_REQ_CLEAR_FEATURE) {
	        /* Special case handling for pipe reset */
		le16_to_cpus(&ctrl_msg->dr.wIndex);
		usb_endpoint_running(adapter->usb_dev,
				     ctrl_msg->dr.wIndex & ~USB_DIR_IN, 
				     (ctrl_msg->dr.wIndex & USB_DIR_IN) == 0);

		/* toggle is reset on clear */
		usb_settoggle(adapter->usb_dev, 
			      ctrl_msg->dr.wIndex & ~USB_DIR_IN, 
			      (ctrl_msg->dr.wIndex & USB_DIR_IN) == 0,
			      0);


	}
	
	if (ctrl_msg->complete)
		ctrl_msg->complete(ctrl_msg->context);

	clear_bit(0, &ctrl->busy);
	
	// Try to send next control message
	usb_next_ctrl_msg(urb, adapter);
	return;
}

/* ======================================================================
 * interrupt pipe
 */

/*
 * The interrupt endpoint will be called when any
 * of the 6 registers changes state (depending on masks).
 * Decode the register values and schedule a private event.
 * Called at interrupt.
 */
static void usb_int_complete(struct urb *urb)
{
	u_char *data = urb->transfer_buffer;
	u_char irqbyte;
	struct st5481_adapter *adapter = urb->context;
	int j;

	if (urb->status < 0) {
		if (urb->status != USB_ST_URB_KILLED) {
			WARN("urb status %d",urb->status);
			urb->actual_length = 0;
		} else {
			DBG(1,"urb killed");
			return; // Give up
		}
	}
	
	DBG_PACKET(1, data, INT_PKT_SIZE);
		
	if (urb->actual_length == 0) {
		return;
	}

	irqbyte = data[MPINT];
	if (irqbyte & DEN_INT)
		FsmEvent(&adapter->d_out.fsm, EV_DOUT_DEN, NULL);

	if (irqbyte & DCOLL_INT)
		FsmEvent(&adapter->d_out.fsm, EV_DOUT_COLL, NULL);

	irqbyte = data[FFINT_D];
	if (irqbyte & OUT_UNDERRUN)
		FsmEvent(&adapter->d_out.fsm, EV_DOUT_UNDERRUN, NULL);

	if (irqbyte & OUT_DOWN)
;//		printk("OUT_DOWN\n");

	irqbyte = data[MPINT];
	if (irqbyte & RXCI_INT)
		FsmEvent(&adapter->l1m, data[CCIST] & 0x0f, NULL);

	for (j = 0; j < 2; j++)
		adapter->bcs[j].b_out.flow_event |= data[FFINT_B1 + j];

	urb->actual_length = 0;
}

/* ======================================================================
 * initialization
 */

int __devinit st5481_setup_usb(struct st5481_adapter *adapter)
{
	struct usb_device *dev = adapter->usb_dev;
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	struct st5481_intr *intr = &adapter->intr;
	struct usb_interface_descriptor *altsetting;
	struct usb_endpoint_descriptor *endpoint;
	int status;
	struct urb *urb;
	u_char *buf;
	
	DBG(1,"");
	
	if ((status = usb_set_configuration (dev,dev->config[0].bConfigurationValue)) < 0) {
		WARN("set_configuration failed,status=%d",status);
		return status;
	}

	
	altsetting = &(dev->config->interface[0].altsetting[3]);	

	// Check if the config is sane
	if ( altsetting->bNumEndpoints != 7 ) {
		WARN("expecting 7 got %d endpoints!", altsetting->bNumEndpoints);
		return -EINVAL;
	}

	// The descriptor is wrong for some early samples of the ST5481 chip
	altsetting->endpoint[3].wMaxPacketSize = 32;
	altsetting->endpoint[4].wMaxPacketSize = 32;

	// Use alternative setting 3 on interface 0 to have 2B+D
	if ((status = usb_set_interface (dev, 0, 3)) < 0) {
		WARN("usb_set_interface failed,status=%d",status);
		return status;
	}

	// Allocate URB for control endpoint
	urb = usb_alloc_urb(0);
	if (!urb) {
		return -ENOMEM;
	}
	ctrl->urb = urb;
	
	// Fill the control URB
	FILL_CONTROL_URB (urb, dev, 
			  usb_sndctrlpipe(dev, 0),
			  NULL, NULL, 0, usb_ctrl_complete, adapter);

		
	fifo_init(&ctrl->msg_fifo.f, ARRAY_SIZE(ctrl->msg_fifo.data));

	// Allocate URBs and buffers for interrupt endpoint
	urb = usb_alloc_urb(0);
	if (!urb) { 
		return -ENOMEM;
	}
	intr->urb = urb;
	
	buf = kmalloc(INT_PKT_SIZE, GFP_KERNEL);
	if (!buf) {
		return -ENOMEM;
	}

	endpoint = &altsetting->endpoint[EP_INT-1];
				
	// Fill the interrupt URB
	FILL_INT_URB(urb, dev,
		     usb_rcvintpipe(dev, endpoint->bEndpointAddress),
		     buf, INT_PKT_SIZE,
		     usb_int_complete, adapter,
		     endpoint->bInterval);
		
	return 0;
}

/*
 * Release buffers and URBs for the interrupt and control
 * endpoint.
 */
void st5481_release_usb(struct st5481_adapter *adapter)
{
	struct st5481_intr *intr = &adapter->intr;
	struct st5481_ctrl *ctrl = &adapter->ctrl;

	DBG(1,"");

	// Stop and free Control and Interrupt URBs
	usb_unlink_urb(ctrl->urb);
	if (ctrl->urb->transfer_buffer)
		kfree(ctrl->urb->transfer_buffer);
	usb_free_urb(ctrl->urb);

	usb_unlink_urb(intr->urb);
	if (intr->urb->transfer_buffer)
		kfree(intr->urb->transfer_buffer);
	usb_free_urb(intr->urb);
}

/*
 *  Initialize the adapter.
 */
void __devinit st5481_start(struct st5481_adapter *adapter)
{
	static const u8 init_cmd_table[]={
		SET_DEFAULT,0,
		STT,0,
		SDA_MIN,0x0d,
		SDA_MAX,0x29,
		SDELAY_VALUE,0x14,
		GPIO_DIR,0x01,		
		GPIO_OUT,RED_LED,
//		FFCTRL_OUT_D,4,
//		FFCTRH_OUT_D,12,
		FFCTRL_OUT_B1,6,
		FFCTRH_OUT_B1,20,
		FFCTRL_OUT_B2,6,
		FFCTRH_OUT_B2,20,
		MPMSK,RXCI_INT+DEN_INT+DCOLL_INT,
		0
	};	
	struct st5481_intr *intr = &adapter->intr;
	int i = 0;
	u8 request,value;

	DBG(8,"");

	adapter->leds = RED_LED; 

	// Start receiving on the interrupt endpoint
	SUBMIT_URB(intr->urb); 

	while ((request = init_cmd_table[i++])) {
		value = init_cmd_table[i++];
		st5481_usb_device_ctrl_msg(adapter, request, value, NULL, NULL);
	}
	st5481_ph_command(adapter, ST5481_CMD_PUP);
}

/*
 * Reset the adapter to default values.
 */
void __devexit st5481_stop(struct st5481_adapter *adapter)
{
	DBG(8,"");

	st5481_usb_device_ctrl_msg(adapter, SET_DEFAULT, 0, NULL, NULL);
}

/* ======================================================================
 * isochronous USB  helpers
 */

static void __devinit
fill_isoc_urb(struct urb *urb, struct usb_device *dev,
	      unsigned int pipe, void *buf, int num_packets, 
	      int packet_size, usb_complete_t complete,
	      void *context) 
{
	int k;

	spin_lock_init(&urb->lock);
	urb->dev=dev;
	urb->pipe=pipe;
	urb->transfer_buffer=buf;
	urb->number_of_packets = num_packets;
	urb->transfer_buffer_length=num_packets*packet_size;
	urb->actual_length = 0;
	urb->complete=complete;
	urb->context=context;
	urb->transfer_flags=USB_ISO_ASAP;
	for (k = 0; k < num_packets; k++) {
		urb->iso_frame_desc[k].offset = packet_size * k;
		urb->iso_frame_desc[k].length = packet_size;
		urb->iso_frame_desc[k].actual_length = 0;
	}
}

int __devinit
st5481_setup_isocpipes(struct urb* urb[2], struct usb_device *dev, 
			   unsigned int pipe, int num_packets,
			   int packet_size, int buf_size,
			   usb_complete_t complete, void *context)
{
	int j, retval;
	unsigned char *buf;

	for (j = 0; j < 2; j++) {
		retval = -ENOMEM;
		urb[j] = usb_alloc_urb(num_packets);
		if (!urb[j])
			goto err;

		// Allocate memory for 2000bytes/sec (16Kb/s)
		buf = kmalloc(buf_size, GFP_KERNEL);
		if (!buf)
			goto err;
			
		// Fill the isochronous URB
		fill_isoc_urb(urb[j], dev, pipe, buf, 
			      num_packets, packet_size, complete,
			      context);
	}
	return 0;

 err:
	for (j = 0; j < 2; j++) {
		if (urb[j]) {
			if (urb[j]->transfer_buffer)
				kfree(urb[j]->transfer_buffer);
			usb_free_urb(urb[j]);
		}
	}
	return retval;
}

void st5481_release_isocpipes(struct urb* urb[2])
{
	int j;

	for (j = 0; j < 2; j++) {
		usb_unlink_urb(urb[j]);
		if (urb[j]->transfer_buffer)
			kfree(urb[j]->transfer_buffer);			
		usb_free_urb(urb[j]);
	}
}

/*
 * Decode frames received on the B/D channel.
 * Note that this function will be called continously
 * with 64Kbit/s / 16Kbit/s of data and hence it will be 
 * called 50 times per second with 20 ISOC descriptors. 
 * Called at interrupt.
 */
static void usb_in_complete(struct urb *urb)
{
	struct st5481_in *in = urb->context;
	unsigned char *ptr;
	struct sk_buff *skb;
	int len, count, status;

	if (urb->status < 0) {
		if (urb->status != USB_ST_URB_KILLED) {
			WARN("urb status %d",urb->status);
		} else {
			DBG(1,"urb killed");
			return; // Give up
		}
	}

	DBG_ISO_PACKET(0x80,urb);

	len = st5481_isoc_flatten(urb);
	ptr = urb->transfer_buffer;
	while (len > 0) {
		if (in->mode == L1_MODE_TRANS) {
			memcpy(in->rcvbuf, ptr, len);
			status = len;
			len = 0;
		} else {
			status = hdlc_decode(&in->hdlc_state, ptr, len, &count,
					     in->rcvbuf, in->bufsize);
			ptr += count;
			len -= count;
		}
		
		if (status > 0) {
			// Good frame received
			DBG(4,"count=%d",status);
			DBG_PACKET(0x400, in->rcvbuf, status);
			if (!(skb = dev_alloc_skb(status))) {
				WARN("receive out of memory\n");
				break;
			}
			memcpy(skb_put(skb, status), in->rcvbuf, status);
			in->hisax_if->l1l2(in->hisax_if, PH_DATA | INDICATION, skb);
		} else if (status == -HDLC_CRC_ERROR) {
			INFO("CRC error");
		} else if (status == -HDLC_FRAMING_ERROR) {
			INFO("framing error");
		} else if (status == -HDLC_LENGTH_ERROR) {
			INFO("length error");
		}
	}

	// Prepare URB for next transfer
	urb->dev = in->adapter->usb_dev;
	urb->actual_length = 0;

	SUBMIT_URB(urb);
}

int __devinit st5481_setup_in(struct st5481_in *in)
{
	struct usb_device *dev = in->adapter->usb_dev;
	int retval;

	DBG(4,"");

	in->rcvbuf = kmalloc(in->bufsize, GFP_KERNEL);
	retval = -ENOMEM;
	if (!in->rcvbuf)
		goto err;

	retval = st5481_setup_isocpipes(in->urb, dev, 
					usb_rcvisocpipe(dev, in->ep),
					in->num_packets,  in->packet_size,
					in->num_packets * in->packet_size,
					usb_in_complete, in);
	if (retval)
		goto err_free;
	return 0;

 err_free:
	kfree(in->rcvbuf);
 err:
	return retval;
}

void st5481_release_in(struct st5481_in *in)
{
	DBG(2,"");

	st5481_release_isocpipes(in->urb);
}

/*
 * Make the transfer_buffer contiguous by
 * copying from the iso descriptors if necessary. 
 */
int st5481_isoc_flatten(struct urb *urb)
{
	struct iso_packet_descriptor *pipd;
	struct iso_packet_descriptor *pend;
	unsigned char *src,*dst;
	unsigned int len;
	
	if (urb->status < 0) {
		return urb->status;
	}
	for (pipd = &urb->iso_frame_desc[0],
		     pend = &urb->iso_frame_desc[urb->number_of_packets],
		     dst = urb->transfer_buffer; 
	     pipd < pend; 
	     pipd++) {
		
		if (pipd->status < 0) {
			return (pipd->status);
		}
	
		len = pipd->actual_length;
		pipd->actual_length = 0;
		src = urb->transfer_buffer+pipd->offset;

		if (src != dst) {
			// Need to copy since isoc buffers not full
			while (len--) {
				*dst++ = *src++;
			}			
		} else {
			// No need to copy, just update destination buffer
			dst += len;
		}
	}
	// Return size of flattened buffer
	return (dst - (unsigned char *)urb->transfer_buffer);
}

static void st5481_start_rcv(void *context)
{
	struct st5481_in *in = context;
	struct st5481_adapter *adapter = in->adapter;

	DBG(4,"");

	in->urb[0]->dev = adapter->usb_dev;
	SUBMIT_URB(in->urb[0]);

	in->urb[1]->dev = adapter->usb_dev;
	SUBMIT_URB(in->urb[1]);
}

void st5481_in_mode(struct st5481_in *in, int mode)
{
	if (in->mode == mode)
		return;

	in->mode = mode;

	usb_unlink_urb(in->urb[0]);
	usb_unlink_urb(in->urb[1]);

	if (in->mode != L1_MODE_NULL) {
		if (in->mode != L1_MODE_TRANS)
			hdlc_rcv_init(&in->hdlc_state,
				      in->mode == L1_MODE_HDLC_56K);
		
		st5481_usb_pipe_reset(in->adapter, in->ep, NULL, NULL);
		st5481_usb_device_ctrl_msg(in->adapter, in->counter,
					   in->packet_size,
					   NULL, NULL);
		st5481_start_rcv(in);
	} else {
		st5481_usb_device_ctrl_msg(in->adapter, in->counter,
					   0, NULL, NULL);
	}
}
