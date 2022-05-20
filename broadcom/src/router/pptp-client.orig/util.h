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
/* util.h ....... error message utilities.
 *                C. Scott Ananian <cananian@alumni.princeton.edu>
 *
 * $Id: util.h,v 1.1.1.1 2007/01/25 12:52:55 jiahao_jhou Exp $
 */

#ifndef INC_UTIL_H
#define INC_UTIL_H

void _log(char *func, char *file, int line, char *format, ...)
     __attribute__ ((format (printf, 4, 5)));
void _warn(char *func, char *file, int line, char *format, ...)
     __attribute__ ((format (printf, 4, 5)));
void _fatal(char *func, char *file, int line, char *format, ...)
     __attribute__ ((format (printf, 4, 5))) __attribute__ ((noreturn));

#define log(format, args...) \
	_log(__FUNCTION__,__FILE__,__LINE__, format , ## args)
#define warn(format, args...) \
	_warn(__FUNCTION__,__FILE__,__LINE__, format , ## args)
#define fatal(format, args...) \
	_fatal(__FUNCTION__,__FILE__,__LINE__, format , ## args)

#endif /* INC_UTIL_H */
