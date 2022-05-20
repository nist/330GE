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
#ifndef	__BB_PWD_H
#define	__BB_PWD_H

#if defined USE_SYSTEM_PWD_GRP
#include <pwd.h>
#else

#include <sys/types.h>
#include <features.h>
#include <stdio.h>

/* The passwd structure.  */
struct passwd
{
  char *pw_name;		/* Username.  */
  char *pw_passwd;		/* Password.  */
  uid_t pw_uid;			/* User ID.  */
  gid_t pw_gid;			/* Group ID.  */
  char *pw_gecos;		/* Real name.  */
  char *pw_dir;			/* Home directory.  */
  char *pw_shell;		/* Shell program.  */
};

extern void setpwent __P ((void));
extern void endpwent __P ((void));
extern struct passwd * getpwent __P ((void));

extern int putpwent __P ((__const struct passwd * __p, FILE * __f));
extern int getpw __P ((uid_t uid, char *buf));

extern struct passwd * fgetpwent __P ((FILE * file));

extern struct passwd * getpwuid __P ((__const uid_t));
extern struct passwd * getpwnam __P ((__const char *));

extern struct passwd * __getpwent __P ((__const int passwd_fd));

#endif /* USE_SYSTEM_PWD_GRP */
#endif /* __BB_PWD_H  */

