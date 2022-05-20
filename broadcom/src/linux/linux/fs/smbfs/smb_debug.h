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
 * Defines some debug macros for smbfs.
 */

/* This makes a dentry parent/child name pair. Useful for debugging printk's */
#define DENTRY_PATH(dentry) \
	(dentry)->d_parent->d_name.name,(dentry)->d_name.name

/*
 * safety checks that should never happen ???
 * these are normally enabled.
 */
#ifdef SMBFS_PARANOIA
# define PARANOIA(f, a...) printk(KERN_NOTICE "%s: " f, __FUNCTION__ , ## a)
#else
# define PARANOIA(f, a...) do { ; } while(0)
#endif

/* lots of debug messages */
#ifdef SMBFS_DEBUG_VERBOSE
# define VERBOSE(f, a...) printk(KERN_DEBUG "%s: " f, __FUNCTION__ , ## a)
#else
# define VERBOSE(f, a...) do { ; } while(0)
#endif

/*
 * "normal" debug messages, but not with a normal DEBUG define ... way
 * too common name.
 */
#ifdef SMBFS_DEBUG
# define DEBUG1(f, a...) printk(KERN_DEBUG "%s: " f, __FUNCTION__ , ## a)
#else
# define DEBUG1(f, a...) do { ; } while(0)
#endif
