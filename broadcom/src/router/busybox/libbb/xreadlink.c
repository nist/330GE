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
 *  xreadlink.c - safe implementation of readlink.
 *  Returns a NULL on failure...
 */

#include <stdio.h>

/*
 * NOTE: This function returns a malloced char* that you will have to free
 * yourself. You have been warned.
 */

#include <unistd.h>
#include "libbb.h"

extern char *xreadlink(const char *path)
{                       
	static const int GROWBY = 80; /* how large we will grow strings by */

	char *buf = NULL;   
	int bufsize = 0, readsize = 0;

	do {
		buf = xrealloc(buf, bufsize += GROWBY);
		readsize = readlink(path, buf, bufsize); /* 1st try */
		if (readsize == -1) {
		    perror_msg("%s:%s", applet_name, path);
		    return NULL;
		}
	}           
	while (bufsize < readsize + 1);

	buf[readsize] = '\0';

	return buf;
}       

