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
#ifndef _ASM_SEGMENT_H
#define _ASM_SEGMENT_H

#define __KERNEL_CS	0x10
#define __KERNEL_DS	0x18

#define __KERNEL32_CS   0x38

/* 
 * we cannot use the same code segment descriptor for user and kernel
 * even not in the long flat model, because of different DPL /kkeil 
 * The segment offset needs to contain a RPL. Grr. -AK
 * GDT layout to get 64bit syscall right (sysret hardcodes gdt offsets) 
 */

#define __USER32_CS   0x23   /* 4*8+3 */ 
#define __USER_DS     0x2b   /* 5*8+3 */ 
#define __USER_CS     0x33   /* 6*8+3 */ 
#define __USER32_DS	__USER_DS 

#endif
