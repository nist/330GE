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
#ifndef _IPTABLES_COMMON_H
#define _IPTABLES_COMMON_H
/* Shared definitions between ipv4 and ipv6. */

enum exittype {
	OTHER_PROBLEM = 1,
	PARAMETER_PROBLEM,
	VERSION_PROBLEM
};
extern void exit_printhelp() __attribute__((noreturn));
extern void exit_tryhelp(int) __attribute__((noreturn));
int check_inverse(const char option[], int *invert, int *optind, int argc);
extern int string_to_number(const char *, 
			    unsigned int, 
			    unsigned int,
			    unsigned int *);
extern int iptables_insmod(const char *modname, const char *modprobe);
void exit_error(enum exittype, char *, ...)__attribute__((noreturn,
							  format(printf,2,3)));
extern const char *program_name, *program_version;

#ifdef NO_SHARED_LIBS
# ifdef _INIT
#  define _init _INIT
# endif
  extern void init_extensions(void);
#endif

#endif /*_IPTABLES_COMMON_H*/
