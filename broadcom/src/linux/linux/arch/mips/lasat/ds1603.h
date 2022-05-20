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
 * Dallas Semiconductors 1603 RTC driver 
 *
 * Brian Murphy <brian@murphy.dk> 
 *
 */
#ifndef __DS1603_H
#define __DS1603_H

struct ds_defs {
	volatile u32 *reg;
	volatile u32 *data_reg;
	u32 rst;
	u32 clk;
	u32 data;
	u32 data_read_shift;
	char data_reversed;
	u32 huge_delay;
};

extern struct ds_defs *ds1603;

unsigned long ds1603_read(void);
void ds1603_set(unsigned long);
void ds1603_set_trimmer(unsigned int);
void ds1603_enable(void);
void ds1603_disable(void);
void ds1603_init(struct ds_defs *);

#define TRIMMER_DEFAULT	3
#define TRIMMER_DISABLE_RTC 0

#endif
