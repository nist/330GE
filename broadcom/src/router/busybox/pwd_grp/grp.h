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
#ifndef	__BB_GRP_H
#define	__BB_GRP_H

#if defined USE_SYSTEM_PWD_GRP
#include <grp.h>
#else

#include <sys/types.h>
#include <features.h>
#include <stdio.h>

/* The group structure */
struct group
{
  char *gr_name;		/* Group name.	*/
  char *gr_passwd;		/* Password.	*/
  gid_t gr_gid;			/* Group ID.	*/
  char **gr_mem;		/* Member list.	*/
};

extern void setgrent __P ((void));
extern void endgrent __P ((void));
extern struct group * getgrent __P ((void));

extern struct group * getgrgid __P ((__const gid_t gid));
extern struct group * getgrnam __P ((__const char * name));

extern struct group * fgetgrent __P ((FILE * file));

extern int setgroups __P ((size_t n, __const gid_t * groups));
extern int initgroups __P ((__const char * user, gid_t gid));

extern struct group * __getgrent __P ((int grp_fd));

#endif /* USE_SYSTEM_PWD_GRP */
#endif /* __BB_GRP_H */

