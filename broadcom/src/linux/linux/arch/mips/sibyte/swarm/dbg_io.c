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
 * kgdb debug routines for swarm board.
 *
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

/* -------------------- BEGINNING OF CONFIG --------------------- */

#include <linux/delay.h>
#include <asm/sibyte/sb1250.h>
#include <asm/sibyte/sb1250_regs.h>
#include <asm/sibyte/sb1250_uart.h>
#include <asm/sibyte/sb1250_int.h>
#include <asm/sibyte/64bit.h>
#include <asm/addrspace.h>

/*
 * We use the second serial port for kgdb traffic.
 * 	115200, 8, N, 1.
 */

#define	BAUD_RATE		115200
#define	CLK_DIVISOR		V_DUART_BAUD_RATE(BAUD_RATE)
#define	DATA_BITS		V_DUART_BITS_PER_CHAR_8		/* or 7    */
#define	PARITY			V_DUART_PARITY_MODE_NONE	/* or even */
#define	STOP_BITS		M_DUART_STOP_BIT_LEN_1		/* or 2    */

static int duart_initialized = 0;	/* 0: need to be init'ed by kgdb */

/* -------------------- END OF CONFIG --------------------- */


#define	duart_out(reg, val)	out64(val, KSEG1 + A_DUART_CHANREG(1,reg))
#define duart_in(reg)		in64(KSEG1 + A_DUART_CHANREG(1,reg))

extern void set_async_breakpoint(unsigned int epc);

void putDebugChar(unsigned char c);
unsigned char getDebugChar(void);
static void
duart_init(int clk_divisor, int data, int parity, int stop)
{
	duart_out(R_DUART_MODE_REG_1, data | parity);
	duart_out(R_DUART_MODE_REG_2, stop);
	duart_out(R_DUART_CLK_SEL, clk_divisor);

	duart_out(R_DUART_CMD, M_DUART_RX_EN | M_DUART_TX_EN);	/* enable rx and tx */
}

void
putDebugChar(unsigned char c)
{
	if (!duart_initialized) {
		duart_initialized = 1;
		duart_init(CLK_DIVISOR, DATA_BITS, PARITY, STOP_BITS);
	}
	while ((duart_in(R_DUART_STATUS) & M_DUART_TX_RDY) == 0);
	duart_out(R_DUART_TX_HOLD, c);
}

unsigned char
getDebugChar(void)
{
	if (!duart_initialized) {
		duart_initialized = 1;
		duart_init(CLK_DIVISOR, DATA_BITS, PARITY, STOP_BITS);
	}
	while ((duart_in(R_DUART_STATUS) & M_DUART_RX_RDY) == 0) ;
	return duart_in(R_DUART_RX_HOLD);
}
