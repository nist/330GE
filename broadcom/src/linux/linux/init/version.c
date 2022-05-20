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
 *  linux/init/version.c
 *
 *  Copyright (C) 1992  Theodore Ts'o
 *
 *  May be freely distributed as part of Linux.
 */

#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/version.h>
#include <linux/compile.h>

#define version(a) Version_ ## a
#define version_string(a) version(a)

int version_string(LINUX_VERSION_CODE);

struct new_utsname system_utsname = {
	UTS_SYSNAME, UTS_NODENAME, UTS_RELEASE, UTS_VERSION,
	UTS_MACHINE, UTS_DOMAINNAME
};

const char *linux_banner = 
	"Linux version " UTS_RELEASE " (" LINUX_COMPILE_BY "@"
	LINUX_COMPILE_HOST ") (" LINUX_COMPILER ") " UTS_VERSION "\n";
