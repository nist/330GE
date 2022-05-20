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
/* util.c ....... error message utilities.
 *                C. Scott Ananian <cananian@alumni.princeton.edu>
 *
 * $Id: util.c,v 1.1.1.1 2007/01/25 12:52:55 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.h"

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "pptp"
#endif

static void open_log(void) __attribute__ ((constructor));
static void close_log(void) __attribute__ ((destructor));

static void open_log(void) {
  openlog(PROGRAM_NAME, LOG_PID, LOG_DAEMON);
}
static void close_log(void) {
  closelog();
}

#define MAKE_STRING(label) 				\
va_list ap;						\
char buf[256], string[256];				\
va_start(ap, format);					\
vsnprintf(buf, sizeof(buf), format, ap);		\
snprintf(string, sizeof(string), "%s[%s:%s:%d]: %s",	\
	 label, func, file, line, buf);			\
va_end(ap)

void _log(char *func, char *file, int line, char *format, ...) {
  MAKE_STRING("log");
  syslog(LOG_NOTICE, "%s", string);
}

void _warn(char *func, char *file, int line, char *format, ...) {
  MAKE_STRING("warn");
  fprintf(stderr, "%s\n", string);
  syslog(LOG_NOTICE, "%s", string);
}

void _fatal(char *func, char *file, int line, char *format, ...) {
  MAKE_STRING("fatal");
  fprintf(stderr, "%s\n", string);
  syslog(LOG_NOTICE, "%s", string);
  exit(1);
}
