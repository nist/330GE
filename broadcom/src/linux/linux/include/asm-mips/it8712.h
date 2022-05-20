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

#ifndef __IT8712_H__
#define __IT8712_H__

#define LPC_BASE_ADDR	0x14000000

// MB PnP configuration register
#define LPC_KEY_ADDR	0x1400002E
#define LPC_DATA_ADDR	0x1400002F

// Device LDN
#define LDN_SERIAL1	0x01
#define LDN_SERIAL2	0x02
#define LDN_PARALLEL	0x03
#define LDN_KEYBOARD	0x05
#define LDN_MOUSE	0x06

#define IT8712_UART1_PORT      0x3F8
#define IT8712_UART2_PORT      0x2F8

#ifndef ASM_ONLY

void LPCSetConfig(char LdnNumber, char Index, char data);
char LPCGetConfig(char LdnNumber, char Index);

#endif

#endif
