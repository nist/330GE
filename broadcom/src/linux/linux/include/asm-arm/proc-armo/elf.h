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
 * ELF definitions for 26-bit CPUs
 */

#define ELF_EXEC_PAGESIZE	32768

#ifdef __KERNEL__

/* We can only execute 26-bit code. */
#define ELF_PROC_OK(x)		\
	((x)->e_flags & EF_ARM_APCS26)

#define SET_PERSONALITY(ex,ibcs2) set_personality(PER_LINUX)

#endif
