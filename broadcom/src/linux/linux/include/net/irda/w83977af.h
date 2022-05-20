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
#ifndef W83977AF_H
#define W83977AF_H

#define W977_EFIO_BASE 0x370
#define W977_EFIO2_BASE 0x3f0
#define W977_DEVICE_IR 0x06


/*
 * Enter extended function mode
 */
static inline void w977_efm_enter(unsigned int efio)
{
        outb(0x87, efio);
        outb(0x87, efio);
}

/*
 * Select a device to configure 
 */

static inline void w977_select_device(__u8 devnum, unsigned int efio)
{
	outb(0x07, efio);
	outb(devnum, efio+1);
} 

/* 
 * Write a byte to a register
 */
static inline void w977_write_reg(__u8 reg, __u8 value, unsigned int efio)
{
	outb(reg, efio);
	outb(value, efio+1);
}

/*
 * read a byte from a register
 */
static inline __u8 w977_read_reg(__u8 reg, unsigned int efio)
{
	outb(reg, efio);
	return inb(efio+1);
}

/*
 * Exit extended function mode
 */
static inline void w977_efm_exit(unsigned int efio)
{
	outb(0xAA, efio);
}
#endif
