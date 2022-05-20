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
#ifndef IPH5526_IP_H
#define IPH5526_IP_H

#define LLC_SNAP_LEN		0x8

/* Offsets into the ARP frame */
#define ARP_OPCODE_0	(0x6 + LLC_SNAP_LEN)
#define ARP_OPCODE_1	(0x7 + LLC_SNAP_LEN)

int iph5526_probe(struct net_device *dev);
static int fcdev_init(struct net_device *dev);
static int iph5526_open(struct net_device *dev);
static int iph5526_close(struct net_device *dev);
static int iph5526_send_packet(struct sk_buff *skb, struct net_device *dev);
static struct net_device_stats * iph5526_get_stats(struct net_device *dev);
static int iph5526_change_mtu(struct net_device *dev, int mtu);


static void rx_net_packet(struct fc_info *fi, u_char *buff_addr, int payload_size);
static void rx_net_mfs_packet(struct fc_info *fi, struct sk_buff *skb);
unsigned short fc_type_trans(struct sk_buff *skb, struct net_device *dev); 
static int tx_ip_packet(struct sk_buff *skb, unsigned long len, struct fc_info *fi);
static int tx_arp_packet(char *data, unsigned long len, struct fc_info *fi);
#endif

