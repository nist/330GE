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
/* $Id: bbc_i2c.h,v 1.1.1.1 2007/01/25 12:51:57 jiahao_jhou Exp $ */
#ifndef _BBC_I2C_H
#define _BBC_I2C_H

#include <asm/ebus.h>

struct bbc_i2c_client;

/* Probing and attachment. */
extern struct linux_ebus_child *bbc_i2c_getdev(int);
extern struct bbc_i2c_client *bbc_i2c_attach(struct linux_ebus_child *);
extern void bbc_i2c_detach(struct bbc_i2c_client *);

/* Register read/write.  NOTE: Blocking! */
extern int bbc_i2c_writeb(struct bbc_i2c_client *, unsigned char val, int off);
extern int bbc_i2c_readb(struct bbc_i2c_client *, unsigned char *byte, int off);
extern int bbc_i2c_write_buf(struct bbc_i2c_client *, char *buf, int len, int off);
extern int bbc_i2c_read_buf(struct bbc_i2c_client *, char *buf, int len, int off);

#endif /* _BBC_I2C_H */
