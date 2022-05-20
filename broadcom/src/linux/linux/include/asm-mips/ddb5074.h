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
 *  include/asm-mips/ddb5074.h -- NEC DDB Vrc-5074 definitions
 *
 *  Copyright (C) 2000 Geert Uytterhoeven <geert@sonycom.com>
 *                     Sony Software Development Center Europe (SDCE), Brussels
 */

extern void ddb5074_led_hex(int hex);
extern void ddb5074_led_d2(int on);
extern void ddb5074_led_d3(int on);

