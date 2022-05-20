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
 *  linux/include/asm-arm/proc-fns.h
 *
 *  Copyright (C) 1997-1999 Russell King
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROCFNS_H
#define __ASM_PROCFNS_H

#ifdef __KERNEL__

#include <linux/config.h>

/*
 * Work out if we need multiple CPU support
 */
#undef MULTI_CPU
#undef CPU_NAME

#ifdef CONFIG_CPU_26
# define CPU_INCLUDE_NAME "asm/cpu-multi26.h"
# define MULTI_CPU
#endif

#ifdef CONFIG_CPU_32
# define CPU_INCLUDE_NAME "asm/cpu-multi32.h"
# ifdef CONFIG_CPU_ARM610
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME arm6
#  endif
# endif
# ifdef CONFIG_CPU_ARM710
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME arm7
#  endif
# endif
# ifdef CONFIG_CPU_ARM720T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME arm720
#  endif
# endif
# ifdef CONFIG_CPU_ARM920T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME arm920
#  endif
# endif
# ifdef CONFIG_CPU_ARM922T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME arm922
#  endif
# endif
# ifdef CONFIG_CPU_ARM926T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME arm926
#  endif
# endif
# ifdef CONFIG_CPU_SA110
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME sa110
#  endif
# endif
# ifdef CONFIG_CPU_SA1100
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME sa1100
#  endif
# endif
#endif

#ifndef MULTI_CPU
#undef CPU_INCLUDE_NAME
#define CPU_INCLUDE_NAME "asm/cpu-single.h"
#endif

#include CPU_INCLUDE_NAME

#endif /* __KERNEL__ */


#endif /* __ASM_PROCFNS_H */
