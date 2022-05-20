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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * sgint23.h: Defines for the SGI INT2 and INT3 chipsets.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1997, 98, 1999, 2000 Ralf Baechle
 * Copyright (C) 1999 Andrew R. Baker (andrewb@uab.edu) - INT2 corrections
 * Copyright (C) 2001 Ladislav Michl (ladis@psi.cz)
 */
#ifndef _ASM_SGI_SGINT23_H
#define _ASM_SGI_SGINT23_H

/* These are the virtual IRQ numbers, we divide all IRQ's into
 * 'spaces', the 'space' determines where and how to enable/disable
 * that particular IRQ on an SGI machine. HPC DMA and MC DMA interrups
 * are not supported this way. Driver is supposed to allocate HPC/MC
 * interrupt as shareable and then look to proper status bit (see
 * HAL2 driver). This will prevent many complications, trust me ;-)
 *	--ladis
 */
#define SGINT_EISA	0	/* INDIGO 2 has 16 EISA irq levels */
#define SGINT_CPU	16	/* MIPS CPU define 8 interrupt sources */
#define SGINT_LOCAL0	24	/* INDY has 8 local0 irq levels */
#define SGINT_LOCAL1	32	/* INDY has 8 local1 irq levels */
#define SGINT_LOCAL2	40	/* INDY has 8 local2 vectored irq levels */
#define SGINT_LOCAL3	48	/* INDY has 8 local3 vectored irq levels */
#define SGINT_END	56	/* End of 'spaces' */

/*
 * Individual interrupt definitions for the INDY and Indigo2
 */

#define SGI_SOFT_0_IRQ	SGINT_CPU + 0
#define SGI_SOFT_1_IRQ	SGINT_CPU + 1
#define SGI_LOCAL_0_IRQ	SGINT_CPU + 2
#define SGI_LOCAL_1_IRQ	SGINT_CPU + 3
#define SGI_8254_0_IRQ	SGINT_CPU + 4
#define SGI_8254_1_IRQ	SGINT_CPU + 5
#define SGI_BUSERR_IRQ	SGINT_CPU + 6
#define SGI_TIMER_IRQ	SGINT_CPU + 7

#define SGI_FIFO_IRQ	SGINT_LOCAL0 + 0	/* FIFO full */
#define SGI_GIO_0_IRQ	SGI_FIFO_IRQ		/* GIO-0 */
#define SGI_WD93_0_IRQ	SGINT_LOCAL0 + 1	/* 1st onboard WD93 */
#define SGI_WD93_1_IRQ	SGINT_LOCAL0 + 2	/* 2nd onboard WD93 */
#define SGI_ENET_IRQ	SGINT_LOCAL0 + 3	/* onboard ethernet */
#define SGI_MCDMA_IRQ	SGINT_LOCAL0 + 4	/* MC DMA done */
#define SGI_PARPORT_IRQ	SGINT_LOCAL0 + 5	/* Parallel port */
#define SGI_GIO_1_IRQ	SGINT_LOCAL0 + 6	/* GE / GIO-1 / 2nd-HPC */
#define SGI_MAP_0_IRQ	SGINT_LOCAL0 + 7	/* Mappable interrupt 0 */

#define SGI_GPL0_IRQ	SGINT_LOCAL1 + 0	/* General Purpose LOCAL1_N<0> */
#define SGI_PANEL_IRQ	SGINT_LOCAL1 + 1	/* front panel */
#define SGI_GPL2_IRQ	SGINT_LOCAL1 + 2	/* General Purpose LOCAL1_N<2> */
#define SGI_MAP_1_IRQ	SGINT_LOCAL1 + 3	/* Mappable interrupt 1 */
#define SGI_HPCDMA_IRQ	SGINT_LOCAL1 + 4	/* HPC DMA done */
#define SGI_ACFAIL_IRQ	SGINT_LOCAL1 + 5	/* AC fail */
#define SGI_VINO_IRQ	SGINT_LOCAL1 + 6	/* Indy VINO */
#define SGI_GIO_2_IRQ	SGINT_LOCAL1 + 7	/* Vert retrace / GIO-2 */

/* Mapped interrupts. These interrupts may be mapped to either 0, or 1
 * We map them to 0 */
#define SGI_VERT_IRQ	SGINT_LOCAL2 + 0	/* INT3: newport vertical status */
#define SGI_EISA_IRQ	SGINT_LOCAL2 + 3	/* EISA interrupts */
#define SGI_KEYBD_IRQ	SGINT_LOCAL2 + 4	/* keyboard */
#define SGI_SERIAL_IRQ	SGINT_LOCAL2 + 5	/* onboard serial */

/* INT2 occupies HPC PBUS slot 4, INT3 uses slot 6. */
#define SGI_INT2_BASE	0x1fbd9000	/* physical */
#define SGI_INT3_BASE	0x1fbd9880	/* physical */

struct sgi_ioc_ints {
#ifdef __MIPSEB__
	unsigned char _unused0[3];
	volatile unsigned char istat0;	/* Interrupt status zero */
#else
	volatile unsigned char istat0;	/* Interrupt status zero */
	unsigned char _unused0[3];
#endif
#define ISTAT0_FFULL	0x01
#define ISTAT0_SCSI0	0x02
#define ISTAT0_SCSI1	0x04
#define ISTAT0_ENET	0x08
#define ISTAT0_GFXDMA	0x10
#define ISTAT0_LPR	0x20
#define ISTAT0_HPC2	0x40
#define ISTAT0_LIO2	0x80

#ifdef __MIPSEB__
	unsigned char _unused1[3];
	volatile unsigned char imask0;	/* Interrupt mask zero */
	unsigned char _unused2[3];
	volatile unsigned char istat1;	/* Interrupt status one */
#else
	volatile unsigned char imask0;	/* Interrupt mask zero */
	unsigned char _unused1[3];
	volatile unsigned char istat1;	/* Interrupt status one */
	unsigned char _unused2[3];
#endif
#define ISTAT1_ISDNI	0x01
#define ISTAT1_PWR	0x02
#define ISTAT1_ISDNH	0x04
#define ISTAT1_LIO3	0x08
#define ISTAT1_HPC3	0x10
#define ISTAT1_AFAIL	0x20
#define ISTAT1_VIDEO	0x40
#define ISTAT1_GIO2	0x80

#ifdef __MIPSEB__
	unsigned char _unused3[3];
	volatile unsigned char imask1;		/* Interrupt mask one */
	unsigned char _unused4[3];
	volatile unsigned char vmeistat;	/* VME interrupt status */
	unsigned char _unused5[3];
	volatile unsigned char cmeimask0;	/* VME interrupt mask zero */
	unsigned char _unused6[3];
	volatile unsigned char cmeimask1;	/* VME interrupt mask one */
	unsigned char _unused7[3];
	volatile unsigned char cmepol;		/* VME polarity */
#else
	volatile unsigned char imask1;		/* Interrupt mask one */
	unsigned char _unused3[3];
	volatile unsigned char vmeistat;	/* VME interrupt status */
	unsigned char _unused4[3];
	volatile unsigned char cmeimask0;	/* VME interrupt mask zero */
	unsigned char _unused5[3];
	volatile unsigned char cmeimask1;	/* VME interrupt mask one */
	unsigned char _unused6[3];
	volatile unsigned char cmepol;		/* VME polarity */
	unsigned char _unused7[3];
#endif
};

struct sgi_ioc_timers {
#ifdef __MIPSEB__
	unsigned char _unused0[3];
	volatile unsigned char tcnt0;	/* counter 0 */
	unsigned char _unused1[3];
	volatile unsigned char tcnt1;	/* counter 1 */
	unsigned char _unused2[3];
	volatile unsigned char tcnt2;	/* counter 2 */
	unsigned char _unused3[3];
	volatile unsigned char tcword;	/* control word */
#else
	volatile unsigned char tcnt0;	/* counter 0 */
	unsigned char _unused0[3];
	volatile unsigned char tcnt1;	/* counter 1 */
	unsigned char _unused1[3];
	volatile unsigned char tcnt2;	/* counter 2 */
	unsigned char _unused2[3];
	volatile unsigned char tcword;	/* control word */
	unsigned char _unused3[3];
#endif
};

/* Timer control word bits. */
#define SGINT_TCWORD_BCD    0x01 /* Use BCD mode for counters */
#define SGINT_TCWORD_MMASK  0x0e /* Mode bitmask. */
#define SGINT_TCWORD_MITC   0x00 /* IRQ on terminal count (doesn't work) */
#define SGINT_TCWORD_MOS    0x02 /* One-shot IRQ mode. */
#define SGINT_TCWORD_MRGEN  0x04 /* Normal rate generation */
#define SGINT_TCWORD_MSWGEN 0x06 /* Square wave generator mode */
#define SGINT_TCWORD_MSWST  0x08 /* Software strobe */
#define SGINT_TCWORD_MHWST  0x0a /* Hardware strobe */
#define SGINT_TCWORD_CMASK  0x30 /* Command mask */
#define SGINT_TCWORD_CLAT   0x00 /* Latch command */
#define SGINT_TCWORD_CLSB   0x10 /* LSB read/write */
#define SGINT_TCWORD_CMSB   0x20 /* MSB read/write */
#define SGINT_TCWORD_CALL   0x30 /* Full counter read/write */
#define SGINT_TCWORD_CNT0   0x00 /* Select counter zero */
#define SGINT_TCWORD_CNT1   0x40 /* Select counter one */
#define SGINT_TCWORD_CNT2   0x80 /* Select counter two */
#define SGINT_TCWORD_CRBCK  0xc0 /* Readback command */

#define SGINT_TCSAMP_COUNTER  10255

struct sgi_int2_regs {
	struct sgi_ioc_ints ints;

	volatile u32 ledbits;		/* LED control bits */
#define INT2_LED_TXCLK		0x01	/* GPI to TXCLK enable */
#define INT2_LED_SERSLCT0	0x02	/* serial port0: 0=apple 1=pc */
#define INT2_LED_SERSLCT1	0x04	/* serial port1: 0=apple 1=pc */
#define INT2_LED_CHEAPER	0x08	/* 0=cheapernet 1=ethernet */
#define INT2_LED_POWEROFF	0x10	/* Power-off request, active high */

#ifdef __MIPSEB__
	unsigned char _unused0[3];
	volatile unsigned char tclear;	/* Timer clear strobe address */
#else
	volatile unsigned char tclear;	/* Timer clear strobe address */
	unsigned char _unused0[3];
#endif
#define INT2_TCLEAR_T0CLR	0x1	/* Clear timer0 IRQ */
#define INT2_TCLEAR_T1CLR	0x2	/* Clear timer1 IRQ */
/* I am guesing there are only two unused registers here
 * but I could be wrong...			- andrewb
 */
/*	u32 _unused[3]; */
	u32 _unused[2];
	struct sgi_ioc_timers timers;
};

struct sgi_int3_regs {
	struct sgi_ioc_ints ints;

#ifdef __MIPSEB__
	unsigned char _unused0[3];
	volatile unsigned char tclear;	/* Timer clear strobe address */
#else
	volatile unsigned char tclear;	/* Timer clear strobe address */
	unsigned char _unused0[3];
#endif
	volatile u32 estatus;		/* Error status reg */
	u32 _unused1[2];
	struct sgi_ioc_timers timers;
};

extern struct sgi_int2_regs *sgi_i2regs;
extern struct sgi_int3_regs *sgi_i3regs;
extern struct sgi_ioc_ints *ioc_icontrol;
extern struct sgi_ioc_timers *ioc_timers;
extern volatile unsigned char *ioc_tclear;

#endif /* _ASM_SGI_SGINT23_H */