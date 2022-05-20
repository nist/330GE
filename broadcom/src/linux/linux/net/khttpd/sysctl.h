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
#ifndef _KHTTPD_INCLUDE_GUARD_SYSCTL_H
#define _KHTTPD_INCLUDE_GUARD_SYSCTL_H

extern char 	sysctl_khttpd_docroot[200];
extern int 	sysctl_khttpd_stop;
extern int 	sysctl_khttpd_start;
extern int 	sysctl_khttpd_unload;
extern int 	sysctl_khttpd_clientport;
extern int 	sysctl_khttpd_permreq;
extern int 	sysctl_khttpd_permforbid;
extern int 	sysctl_khttpd_logging;
extern int 	sysctl_khttpd_serverport;
extern int 	sysctl_khttpd_sloppymime;
extern int 	sysctl_khttpd_threads;
extern int	sysctl_khttpd_maxconnect;

/* incremented each time sysctl_khttpd_stop goes nonzero */
extern atomic_t	khttpd_stopCount;

#endif
