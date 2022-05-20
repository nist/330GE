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
#ifndef __BEN_VLAN_PROC_INC__
#define __BEN_VLAN_PROC_INC__

int vlan_proc_init(void);

int vlan_proc_rem_dev(struct net_device *vlandev);
int vlan_proc_add_dev (struct net_device *vlandev);
void vlan_proc_cleanup (void);

#define	VLAN_PROC_BUFSZ	(4096)	/* buffer size for printing proc info */

#endif /* !(__BEN_VLAN_PROC_INC__) */
