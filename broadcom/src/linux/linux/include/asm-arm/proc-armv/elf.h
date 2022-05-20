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
 * ELF definitions for 32-bit CPUs
 */

#define ELF_EXEC_PAGESIZE	4096

#ifdef __KERNEL__

/* 32-bit code is always OK.  Some cpus can do 26-bit, some can't.  */
#define ELF_PROC_OK(x)				\
	((! ((x)->e_flags & EF_ARM_APCS26))	\
	|| (elf_hwcap & HWCAP_26BIT))

/* Old NetWinder binaries were compiled in such a way that the iBCS
   heuristic always trips on them.  Until these binaries become uncommon
   enough not to care, don't trust the `ibcs' flag here.  In any case
   there is no other ELF system currently supported by iBCS.
   @@ Could print a warning message to encourage users to upgrade.  */
#define SET_PERSONALITY(ex,ibcs2) \
	set_personality(((ex).e_flags&EF_ARM_APCS26 ?PER_LINUX :PER_LINUX_32BIT))

#endif
