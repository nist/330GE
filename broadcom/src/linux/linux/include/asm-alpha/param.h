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
#ifndef _ASM_ALPHA_PARAM_H
#define _ASM_ALPHA_PARAM_H

/* ??? Gross.  I don't want to parameterize this, and supposedly the
   hardware ignores reprogramming.  We also need userland buy-in to the 
   change in HZ, since this is visible in the wait4 resources etc.  */

#include <linux/config.h>

#ifndef HZ
# ifndef CONFIG_ALPHA_RAWHIDE
#  define HZ	1024
# else
#  define HZ	1200
# endif
#ifdef __KERNEL__
# define hz_to_std(a) (a)
#endif
#endif

#define EXEC_PAGESIZE	8192

#ifndef NGROUPS
#define NGROUPS		32
#endif

#ifndef NOGROUP
#define NOGROUP		(-1)
#endif

#define MAXHOSTNAMELEN	64	/* max length of hostname */

#ifdef __KERNEL__
# define CLOCKS_PER_SEC	HZ	/* frequency at which times() counts */
#endif

#endif /* _ASM_ALPHA_PARAM_H */
