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
#ifndef __MATROXFB_MAVEN_H__
#define __MATROXFB_MAVEN_H__

#include <linux/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include "matroxfb_base.h"

struct matroxfb_dh_maven_info;

struct i2c_bit_adapter {
	struct i2c_adapter		adapter;
	int				initialized;
	struct i2c_algo_bit_data	bac;
	struct matroxfb_dh_maven_info  *minfo;
};

struct matroxfb_dh_maven_info {
	struct matrox_fb_info*	primary_dev;

	struct i2c_bit_adapter	maven;
	struct i2c_bit_adapter	ddc1;
	struct i2c_bit_adapter	ddc2;
};

#endif /* __MATROXFB_MAVEN_H__ */
