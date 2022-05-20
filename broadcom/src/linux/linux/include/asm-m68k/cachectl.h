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
#ifndef _M68K_CACHECTL_H
#define _M68K_CACHECTL_H

/* Definitions for the cacheflush system call.  */

#define FLUSH_SCOPE_LINE    1	/* Flush a cache line */
#define FLUSH_SCOPE_PAGE    2	/* Flush a page */
#define FLUSH_SCOPE_ALL     3	/* Flush the whole cache -- superuser only */

#define FLUSH_CACHE_DATA    1	/* Writeback and flush data cache */
#define FLUSH_CACHE_INSN    2	/* Flush instruction cache */
#define FLUSH_CACHE_BOTH    3	/* Flush both caches */

#endif /* _M68K_CACHECTL_H */
