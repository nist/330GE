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
 * crc32.h for early Linux 2.4.19pre kernel inclusion
 * This defines ether_crc_le() and ether_crc() as inline functions
 * This is slated to change to using the library crc32 functions
 * as kernel 2.5.2 included at some future date.
 */
#ifndef _LINUX_CRC32_H
#define _LINUX_CRC32_H

#include <linux/types.h>

/* The little-endian AUTODIN II ethernet CRC calculation.
   N.B. Do not use for bulk data, use a table-based routine instead.
   This is common code and should be moved to net/core/crc.c */
static unsigned const ethernet_polynomial_le = 0xedb88320U;
static inline unsigned ether_crc_le(int length, unsigned char *data)
{
	unsigned int crc = 0xffffffff;	/* Initial value. */
	while(--length >= 0) {
		unsigned char current_octet = *data++;
		int bit;
		for (bit = 8; --bit >= 0; current_octet >>= 1) {
			if ((crc ^ current_octet) & 1) {
				crc >>= 1;
				crc ^= ethernet_polynomial_le;
			} else
				crc >>= 1;
		}
	}
	return crc;
}

static unsigned const ethernet_polynomial = 0x04c11db7U;
static inline u32 ether_crc(int length, unsigned char *data)
{
	int crc = -1;
	while (--length >= 0) {
		unsigned char current_octet = *data++;
		int bit;
		for (bit = 0; bit < 8; bit++, current_octet >>= 1) {
			crc = (crc << 1) ^
				((crc < 0) ^ (current_octet & 1) ?
				 ethernet_polynomial : 0);
		}
	}
	return crc;
}

#endif /* _LINUX_CRC32_H */
