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
/* $Id: shadows.c,v 1.1.1.1 2007/01/25 12:51:48 jiahao_jhou Exp $
 * 
 * Various shadow registers. Defines for these are in include/asm-etrax100/io.h
 */

/* Shadows for internal Etrax-registers */

unsigned long genconfig_shadow;
unsigned long port_g_data_shadow;
unsigned char port_pa_dir_shadow;
unsigned char port_pa_data_shadow;
unsigned char port_pb_i2c_shadow;
unsigned char port_pb_config_shadow;
unsigned char port_pb_dir_shadow;
unsigned char port_pb_data_shadow;
unsigned long r_timer_ctrl_shadow;

/* Shadows for external I/O port registers.
 * These are only usable if there actually IS a latch connected
 * to the corresponding external chip-select pin.
 *
 * A common usage is that CSP0 controls LED's and CSP4 video chips.
 */

unsigned long port_cse1_shadow;
unsigned long port_csp0_shadow;
unsigned long port_csp4_shadow;

/* Corresponding addresses for the ports.
 * These are initialized in arch/cris/mm/init.c using ioremap.
 */

volatile unsigned long *port_cse1_addr;
volatile unsigned long *port_csp0_addr;
volatile unsigned long *port_csp4_addr;

