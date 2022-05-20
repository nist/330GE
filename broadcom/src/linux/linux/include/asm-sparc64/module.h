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
#ifndef _ASM_SPARC64_MODULE_H
#define _ASM_SPARC64_MODULE_H
/*
 * This file contains the sparc64 architecture specific module code.
 */

extern void * module_map (unsigned long size);
extern void module_unmap (void *addr);
#define module_arch_init(x)	(0)
#define arch_init_modules(x)	do { } while (0)

#endif /* _ASM_SPARC64_MODULE_H */
