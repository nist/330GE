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
#ifndef __LIBATTR_H
#define __LIBATTR_H

#ifdef __cplusplus
extern "C" {
#endif

struct error_context;

extern int attr_copy_file (const char *, const char *,
			   int (*) (const char *, struct error_context *),
			   struct error_context *);
extern int attr_copy_fd (const char *, int, const char *, int,
			 int (*) (const char *, struct error_context *),
			 struct error_context *);

/* The default check function used by attr_copy_{fd,file}. */
extern int attr_copy_check_permissions(const char *, struct error_context *);

#ifdef __cplusplus
}
#endif

#endif
