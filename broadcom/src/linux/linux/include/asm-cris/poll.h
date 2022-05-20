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
#ifndef __ASM_CRIS_POLL_H
#define __ASM_CRIS_POLL_H

/* taken from asm-alpha */

#define POLLIN		1
#define POLLPRI		2
#define POLLOUT		4
#define POLLERR		8
#define POLLHUP		16
#define POLLNVAL	32
#define POLLRDNORM	64
#define POLLRDBAND	128
#define POLLWRNORM	256
#define POLLWRBAND	512
#define POLLMSG		1024

struct pollfd {
	int fd;
	short events;
	short revents;
};

#endif
