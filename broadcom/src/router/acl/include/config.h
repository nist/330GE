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
/* include/config.h.  Generated automatically by configure.  */
/* Define if you have the attr/error_context.h header */
/* #undef HAVE_ATTR_ERROR_CONTEXT_H */

/* Define if you want Posix compliant getfacl and setfacl utilities
   without extensions */
/* #undef POSIXLY_CORRECT */

/* The number of bytes in a int.  */
/* #undef SIZEOF_INT */

/* The number of bytes in a long.  */
/* #undef SIZEOF_LONG */

/* The number of bytes in a short.  */
/* #undef SIZEOF_SHORT */

/* Define if you have attribute((visibility(hidden))) in gcc. */
#define HAVE_VISIBILITY_ATTRIBUTE 1

/* Define if you want gettext (I18N) support */
/* #undef ENABLE_GETTEXT */

#ifdef ENABLE_GETTEXT
# include <libintl.h>
# define _(x)			gettext(x)
#else
# define _(x)			(x)
# define textdomain(d)		do { } while (0)
# define bindtextdomain(d,dir)	do { } while (0)
#endif
#include <locale.h>

