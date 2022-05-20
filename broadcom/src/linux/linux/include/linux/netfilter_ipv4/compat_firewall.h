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
/* Minor modifications to fit on compatibility framework:
   Rusty.Russell@rustcorp.com.au
*/

#ifndef __LINUX_FIREWALL_H
#define __LINUX_FIREWALL_H

/*
 *	Definitions for loadable firewall modules
 */

#define FW_QUEUE	0
#define FW_BLOCK	1
#define FW_ACCEPT	2
#define FW_REJECT	(-1)
#define FW_REDIRECT	3
#define FW_MASQUERADE	4
#define FW_SKIP		5

struct firewall_ops
{
	struct firewall_ops *next;
	int (*fw_forward)(struct firewall_ops *this, int pf,
			  struct net_device *dev, void *phdr, void *arg,
			  struct sk_buff **pskb);
	int (*fw_input)(struct firewall_ops *this, int pf,
			struct net_device *dev, void *phdr, void *arg,
			struct sk_buff **pskb);
	int (*fw_output)(struct firewall_ops *this, int pf,
			 struct net_device *dev, void *phdr, void *arg,
			 struct sk_buff **pskb);
	/* These may be NULL. */
	int (*fw_acct_in)(struct firewall_ops *this, int pf,
			  struct net_device *dev, void *phdr, void *arg,
			  struct sk_buff **pskb);
	int (*fw_acct_out)(struct firewall_ops *this, int pf,
			   struct net_device *dev, void *phdr, void *arg,
			   struct sk_buff **pskb);
};

extern int register_firewall(int pf, struct firewall_ops *fw);
extern int unregister_firewall(int pf, struct firewall_ops *fw);

extern int ip_fw_masq_timeouts(void *user, int len);
#endif /* __LINUX_FIREWALL_H */
