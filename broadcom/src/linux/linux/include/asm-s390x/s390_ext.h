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
#ifndef _S390_EXTINT_H
#define _S390_EXTINT_H

/*
 *  include/asm-s390/s390_ext.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Holger Smolinski (Holger.Smolinski@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

typedef void (*ext_int_handler_t)(struct pt_regs *regs, __u16 code);

/*
 * Warning: if you change ext_int_info_t you have to change the
 * external interrupt handler in entry.S too.
 */ 
typedef struct ext_int_info_t {
	struct ext_int_info_t *next;
	ext_int_handler_t handler;
	__u16 code;
} __attribute__ ((packed)) ext_int_info_t;

extern ext_int_info_t *ext_int_hash[];

int register_external_interrupt(__u16 code, ext_int_handler_t handler);
int register_early_external_interrupt(__u16 code, ext_int_handler_t handler,
				      ext_int_info_t *info);
int unregister_external_interrupt(__u16 code, ext_int_handler_t handler);
int unregister_early_external_interrupt(__u16 code, ext_int_handler_t handler,
					ext_int_info_t *info);

#endif
