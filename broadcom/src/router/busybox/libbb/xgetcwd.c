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
 * xgetcwd.c -- return current directory with unlimited length
 * Copyright (C) 1992, 1996 Free Software Foundation, Inc.
 * Written by David MacKenzie <djm@gnu.ai.mit.edu>.
 *
 * Special function for busybox written by Vladimir Oleynik <vodz@usa.net>
*/

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include "libbb.h"

/* Amount to increase buffer size by in each try. */
#define PATH_INCR 32

/* Return the current directory, newly allocated, arbitrarily long.
   Return NULL and set errno on error.
   If argument is not NULL (previous usage allocate memory), call free()
*/

char *
xgetcwd (char *cwd)
{
  char *ret;
  unsigned path_max;

  errno = 0;
  path_max = (unsigned) PATH_MAX;
  path_max += 2;                /* The getcwd docs say to do this. */

  if(cwd==0)
	cwd = xmalloc (path_max);

  errno = 0;
  while ((ret = getcwd (cwd, path_max)) == NULL && errno == ERANGE) {
      path_max += PATH_INCR;
      cwd = xrealloc (cwd, path_max);
      errno = 0;
  }

  if (ret == NULL) {
      int save_errno = errno;
      free (cwd);
      errno = save_errno;
      perror_msg("getcwd()");
      return NULL;
  }

  return cwd;
}
