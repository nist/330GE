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
#ifndef _LINUX_HIGHUID_H
#define _LINUX_HIGHUID_H

#include <linux/config.h>
#include <linux/types.h>

/*
 * general notes:
 *
 * CONFIG_UID16 is defined if the given architecture needs to
 * support backwards compatibility for old system calls.
 *
 * kernel code should use uid_t and gid_t at all times when dealing with
 * kernel-private data.
 *
 * old_uid_t and old_gid_t should only be different if CONFIG_UID16 is
 * defined, else the platform should provide dummy typedefs for them
 * such that they are equivalent to __kernel_{u,g}id_t.
 *
 * uid16_t and gid16_t are used on all architectures. (when dealing
 * with structures hard coded to 16 bits, such as in filesystems)
 */


/*
 * This is the "overflow" UID and GID. They are used to signify uid/gid
 * overflow to old programs when they request uid/gid information but are
 * using the old 16 bit interfaces.
 * When you run a libc5 program, it will think that all highuid files or
 * processes are owned by this uid/gid.
 * The idea is that it's better to do so than possibly return 0 in lieu of
 * 65536, etc.
 */

extern int overflowuid;
extern int overflowgid;

#define DEFAULT_OVERFLOWUID	65534
#define DEFAULT_OVERFLOWGID	65534

#ifdef CONFIG_UID16

/* prevent uid mod 65536 effect by returning a default value for high UIDs */
#define high2lowuid(uid) ((uid) > 65535 ? (old_uid_t)overflowuid : (old_uid_t)(uid))
#define high2lowgid(gid) ((gid) > 65535 ? (old_gid_t)overflowgid : (old_gid_t)(gid))
/*
 * -1 is different in 16 bits than it is in 32 bits
 * these macros are used by chown(), setreuid(), ...,
 */
#define low2highuid(uid) ((uid) == (old_uid_t)-1 ? (uid_t)-1 : (uid_t)(uid))
#define low2highgid(gid) ((gid) == (old_gid_t)-1 ? (gid_t)-1 : (gid_t)(gid))

/* Avoid extra ifdefs with these macros */

#define SET_UID16(var, uid)	var = high2lowuid(uid)
#define SET_GID16(var, gid)	var = high2lowgid(gid)
#define NEW_TO_OLD_UID(uid)	high2lowuid(uid)
#define NEW_TO_OLD_GID(gid)	high2lowgid(gid)

/* specific to fs/stat.c */
#define SET_OLDSTAT_UID(stat, uid)	(stat).st_uid = high2lowuid(uid)
#define SET_OLDSTAT_GID(stat, gid)	(stat).st_gid = high2lowgid(gid)
#define SET_STAT_UID(stat, uid)		(stat).st_uid = high2lowuid(uid)
#define SET_STAT_GID(stat, gid)		(stat).st_gid = high2lowgid(gid)

#else

#define SET_UID16(var, uid)	do { ; } while (0)
#define SET_GID16(var, gid)	do { ; } while (0)
#define NEW_TO_OLD_UID(uid)	(uid)
#define NEW_TO_OLD_GID(gid)	(gid)

#define SET_OLDSTAT_UID(stat, uid)	(stat).st_uid = (uid)
#define SET_OLDSTAT_GID(stat, gid)	(stat).st_gid = (gid)
#define SET_STAT_UID(stat, uid)		(stat).st_uid = (uid)
#define SET_STAT_GID(stat, gid)		(stat).st_gid = (gid)

#endif /* CONFIG_UID16 */


/*
 * Everything below this line is needed on all architectures, to deal with
 * filesystems that only store 16 bits of the UID/GID, etc.
 */

/*
 * This is the UID and GID that will get written to disk if a filesystem
 * only supports 16-bit UIDs and the kernel has a high UID/GID to write
 */
extern int fs_overflowuid;
extern int fs_overflowgid;

#define DEFAULT_FS_OVERFLOWUID	65534
#define DEFAULT_FS_OVERFLOWGID	65534

/*
 * Since these macros are used in architectures that only need limited
 * 16-bit UID back compatibility, we won't use old_uid_t and old_gid_t
 */
#define fs_high2lowuid(uid) ((uid) > 65535 ? (uid16_t)fs_overflowuid : (uid16_t)(uid))
#define fs_high2lowgid(gid) ((gid) > 65535 ? (gid16_t)fs_overflowgid : (gid16_t)(gid))

#define low_16_bits(x)	((x) & 0xFFFF)
#define high_16_bits(x)	(((x) & 0xFFFF0000) >> 16)

#endif /* _LINUX_HIGHUID_H */