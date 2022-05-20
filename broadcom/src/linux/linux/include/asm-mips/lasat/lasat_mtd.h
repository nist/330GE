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
#include <linux/types.h>

enum lasat_mtdparts {
	LASAT_MTD_BOOTLOADER,
	LASAT_MTD_SERVICE,
	LASAT_MTD_NORMAL,
	LASAT_MTD_FS,
	LASAT_MTD_CONFIG,
	LASAT_MTD_LAST
}; 

#define BOOTLOADER_SIZE 0x40000

extern unsigned long lasat_flash_partition_start(int partno);
extern unsigned long lasat_flash_partition_size(int partno);
