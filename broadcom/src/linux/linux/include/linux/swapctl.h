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
#ifndef _LINUX_SWAPCTL_H
#define _LINUX_SWAPCTL_H

typedef struct pager_daemon_v1
{
	unsigned int	tries_base;
	unsigned int	tries_min;
	unsigned int	swap_cluster;
} pager_daemon_v1;
typedef pager_daemon_v1 pager_daemon_t;
extern pager_daemon_t pager_daemon;

#endif /* _LINUX_SWAPCTL_H */
