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


#include "hfs.h"

/*================ Global functions ================*/

/*
 * hfs_find_zero_bit()
 *
 * Description:
 *  Given a block of memory, its length in bits, and a starting bit number,
 *  determine the number of the first zero bits (in left-to-right ordering)
 *  in that range.
 *
 *  Returns >= 'size' if no zero bits are found in the range.
 *
 *  Accesses memory in 32-bit aligned chunks of 32-bits and thus
 *  may read beyond the 'size'th bit.
 */
hfs_u32 hfs_find_zero_bit(const hfs_u32 *start, hfs_u32 size, hfs_u32 offset)
{
	const hfs_u32 *end   = start + ((size + 31) >> 5);
	const hfs_u32 *curr  = start + (offset >> 5);
	int bit = offset % 32;
	
	if (offset < size) {
		/* scan the first partial hfs_u32 for zero bits */
		if (bit != 0) {
			do {
				if (!hfs_test_bit(bit, curr)) {
					goto done;
				}
				++bit;
			} while (bit < 32);
			bit = 0;
			++curr;
		}
	
		/* scan complete hfs_u32s for the first zero bit */
		while (curr < end) {
			if (*curr == ~((hfs_u32)0)) {
				++curr;
			} else {
				while (hfs_test_bit(bit, curr)) {
					++bit;
				}
				break;
			}
		}

done:
		bit |= (curr - start) << 5;
		return bit;
	} else {
		return size;
	}
}

/*
 * hfs_count_zero_bits()
 *
 * Description:
 *  Given a block of memory, its length in bits, and a starting bit number,
 *  determine the number of consecutive zero bits (in left-to-right ordering)
 *  in that range.
 *
 *  Accesses memory in 32-bit aligned chunks of 32-bits and thus
 *  may read beyond the 'size'th bit.
 */
hfs_u32 hfs_count_zero_bits(const hfs_u32 *start, hfs_u32 size, hfs_u32 offset)
{
	const hfs_u32 *end   = start + ((size + 31) >> 5);
	const hfs_u32 *curr  = start + (offset >> 5);
	int bit = offset % 32;

	if (offset < size) {
		/* scan the first partial hfs_u32 for one bits */
		if (bit != 0) {
			do {
				if (hfs_test_bit(bit, curr)) {
					goto done;
				}
				++bit;
			} while (bit < 32);
			bit = 0;
			++curr;
		}
	
		/* scan complete hfs_u32s for the first one bit */
		while (curr < end) {
			if (*curr == ((hfs_u32)0)) {
				++curr;
			} else {
				while (!hfs_test_bit(bit, curr)) {
					++bit;
				}
				break;
			}
		}

done:
		bit |= (curr - start) << 5;
		if (bit > size) {
			bit = size;
		}
		return bit - offset;
	} else {
		return 0;
	}
}
