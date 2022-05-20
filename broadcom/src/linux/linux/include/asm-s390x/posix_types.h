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
 *  include/asm-s390/posix_types.h
 *
 *  S390 version
 *
 *  Derived from "include/asm-i386/posix_types.h"
 */

#ifndef __ARCH_S390_POSIX_TYPES_H
#define __ARCH_S390_POSIX_TYPES_H

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

typedef unsigned int    __kernel_dev_t;
typedef unsigned int    __kernel_ino_t;
typedef unsigned int    __kernel_mode_t;
typedef unsigned int    __kernel_nlink_t;
typedef long            __kernel_off_t;
typedef long long       __kernel_loff_t;
typedef int             __kernel_pid_t;
typedef int             __kernel_ipc_pid_t;
typedef unsigned int    __kernel_uid_t;
typedef unsigned int    __kernel_gid_t;
typedef unsigned long   __kernel_size_t;
typedef long            __kernel_ssize_t;
typedef long            __kernel_ptrdiff_t;
typedef long            __kernel_time_t;
typedef long            __kernel_suseconds_t;
typedef long            __kernel_clock_t;
typedef int             __kernel_daddr_t;
typedef char *          __kernel_caddr_t;
typedef unsigned long   __kernel_sigset_t;      /* at least 32 bits */
typedef unsigned short	__kernel_uid16_t;
typedef unsigned short	__kernel_gid16_t;

typedef __kernel_uid_t __kernel_old_uid_t;
typedef __kernel_gid_t __kernel_old_gid_t;
typedef __kernel_uid_t __kernel_uid32_t;
typedef __kernel_gid_t __kernel_gid32_t;

typedef struct {
#if defined(__KERNEL__) || defined(__USE_ALL)
        int     val[2];
#else                        /* !defined(__KERNEL__) && !defined(__USE_ALL)*/
        int     __val[2];
#endif                       /* !defined(__KERNEL__) && !defined(__USE_ALL)*/
} __kernel_fsid_t;


#if defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)

#ifndef _S390_BITOPS_H
#include <asm/bitops.h>
#endif

#undef  __FD_SET
#define __FD_SET(fd,fdsetp)  set_bit(fd,fdsetp)

#undef  __FD_CLR
#define __FD_CLR(fd,fdsetp)  clear_bit(fd,fdsetp)

#undef  __FD_ISSET
#define __FD_ISSET(fd,fdsetp)  test_bit(fd,fdsetp)

#undef  __FD_ZERO
#define __FD_ZERO(fdsetp) (memset (fdsetp, 0, sizeof(*(fd_set *)fdsetp)))

#endif     /* defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)*/

#endif