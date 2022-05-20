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
 * Various TURBOchannel related stuff
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Information obtained through the get_tcinfo prom call
 * created from:
 *
 * TURBOchannel Firmware Specification
 *
 * EK-TCAAD-FS-004
 * from Digital Equipment Corporation
 *
 * Copyright (c) 1998 Harald Koerfgen
 */

typedef struct {
	int revision;
	int clk_period;
	int slot_size;
	int io_timeout;
	int dma_range;
	int max_dma_burst;
	int parity;
	int reserved[4];
} tcinfo;

#define MAX_SLOT 7

typedef struct {
	unsigned long base_addr;
	unsigned char name[9];
	unsigned char vendor[9];
	unsigned char firmware[9];
	int interrupt;
	int flags;
} slot_info;

/*
 * Values for flags
 */
#define FREE 	1<<0
#define IN_USE	1<<1


