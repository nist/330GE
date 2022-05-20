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
/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * create raw socket for icmp protocol test permision
 * and drop root privilegies if running setuid
 *
 */

#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "libbb.h"

int create_icmp_socket(void)
{
	struct protoent *proto;
	int sock;

	proto = getprotobyname("icmp");
	/* if getprotobyname failed, just silently force
	 * proto->p_proto to have the correct value for "icmp" */
	if ((sock = socket(AF_INET, SOCK_RAW,
			(proto ? proto->p_proto : 1))) < 0) {        /* 1 == ICMP */
		if (errno == EPERM)
			error_msg_and_die("permission denied. (are you root?)");
		else
			perror_msg_and_die(can_not_create_raw_socket);
	}

	/* drop root privs if running setuid */
	setuid(getuid());

	return sock;
}
