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
#ifndef _LINUX_SECUREBITS_H
#define _LINUX_SECUREBITS_H 1

#define SECUREBITS_DEFAULT 0x00000000

extern unsigned securebits;

/* When set UID 0 has no special privileges. When unset, we support
   inheritance of root-permissions and suid-root executablew under
   compatibility mode. We raise the effective and inheritable bitmasks
   *of the executable file* if the effective uid of the new process is
   0. If the real uid is 0, we raise the inheritable bitmask of the
   executable file. */
#define SECURE_NOROOT            0

/* When set, setuid to/from uid 0 does not trigger capability-"fixes"
   to be compatible with old programs relying on set*uid to loose
   privileges. When unset, setuid doesn't change privileges. */
#define SECURE_NO_SETUID_FIXUP   2

/* Each securesetting is implemented using two bits. One bit specify
   whether the setting is on or off. The other bit specify whether the
   setting is fixed or not. A setting which is fixed cannot be changed
   from user-level. */

#define issecure(X) ( (1 << (X+1)) & SECUREBITS_DEFAULT ? 	\
		      (1 << (X)) & SECUREBITS_DEFAULT :		\
		      (1 << (X)) & securebits )

#endif /* !_LINUX_SECUREBITS_H */
