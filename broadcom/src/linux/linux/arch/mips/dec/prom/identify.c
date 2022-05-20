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
 * identify.c: machine identification code.
 *
 * Copyright (C) 1998 Harald Koerfgen and Paul M. Antoine
 * Copyright (C) 2002  Maciej W. Rozycki
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mc146818rtc.h>
#include <linux/string.h>
#include <linux/types.h>

#include <asm/bootinfo.h>
#include <asm/dec/ioasic.h>
#include <asm/dec/ioasic_addrs.h>
#include <asm/dec/kn01.h>
#include <asm/dec/kn02.h>
#include <asm/dec/kn02ba.h>
#include <asm/dec/kn02ca.h>
#include <asm/dec/kn03.h>
#include <asm/dec/kn230.h>
#include <asm/dec/prom.h>

#include "dectypes.h"

extern unsigned long mips_machgroup;
extern unsigned long mips_machtype;

static const char *dec_system_strings[] = {
	[MACH_DSUNKNOWN]	"unknown DECstation",
	[MACH_DS23100]		"DECstation 2100/3100",
	[MACH_DS5100]		"DECsystem 5100",
	[MACH_DS5000_200]	"DECstation 5000/200",
	[MACH_DS5000_1XX]	"DECstation 5000/1xx",
	[MACH_DS5000_XX]	"Personal DECstation 5000/xx",
	[MACH_DS5000_2X0]	"DECstation 5000/2x0",
	[MACH_DS5400]		"DECsystem 5400",
	[MACH_DS5500]		"DECsystem 5500",
	[MACH_DS5800]		"DECsystem 5800",
	[MACH_DS5900]		"DECsystem 5900",
};

const char *get_system_type(void)
{
#define STR_BUF_LEN	64
	static char system[STR_BUF_LEN];
	static int called = 0;

	if (called == 0) {
		called = 1;
		snprintf(system, STR_BUF_LEN, "Digital %s",
			 dec_system_strings[mips_machtype]);
	}

	return system;
}


/*
 * Setup essential system-specific memory addresses.  We need them
 * early.  Semantically the functions belong to prom/init.c, but they
 * are compact enough we want them inlined. -- macro
 */
static inline void prom_init_kn01(void)
{
	dec_rtc_base = (void *)KN01_RTC_BASE;
	dec_kn_slot_size = KN01_SLOT_SIZE;
}

static inline void prom_init_kn230(void)
{
	dec_rtc_base = (void *)KN01_RTC_BASE;
	dec_kn_slot_size = KN01_SLOT_SIZE;
}

static inline void prom_init_kn02(void)
{
	dec_rtc_base = (void *)KN02_RTC_BASE;
	dec_kn_slot_size = KN02_SLOT_SIZE;
}

static inline void prom_init_kn02ba(void)
{
	ioasic_base = (void *)KN02BA_IOASIC_BASE;
	dec_rtc_base = (void *)KN02BA_RTC_BASE;
	dec_kn_slot_size = IOASIC_SLOT_SIZE;
}

static inline void prom_init_kn02ca(void)
{
	ioasic_base = (void *)KN02CA_IOASIC_BASE;
	dec_rtc_base = (void *)KN02CA_RTC_BASE;
	dec_kn_slot_size = IOASIC_SLOT_SIZE;
}

static inline void prom_init_kn03(void)
{
	ioasic_base = (void *)KN03_IOASIC_BASE;
	dec_rtc_base = (void *)KN03_RTC_BASE;
	dec_kn_slot_size = IOASIC_SLOT_SIZE;
}


void __init prom_identify_arch(u32 magic)
{
	unsigned char dec_cpunum, dec_firmrev, dec_etc, dec_systype;
	u32 dec_sysid;

	if (!prom_is_rex(magic)) {
		dec_sysid = simple_strtoul(prom_getenv("systype"), (char **)0, 0);
	} else {
		dec_sysid = rex_getsysid();
		if (dec_sysid == 0) {
			prom_printf("Zero sysid returned from PROMs! Assuming PMAX-like machine.\n");
			dec_sysid = 1;
		}
	}

	dec_cpunum = (dec_sysid & 0xff000000) >> 24;
	dec_systype = (dec_sysid & 0xff0000) >> 16;
	dec_firmrev = (dec_sysid & 0xff00) >> 8;
	dec_etc = dec_sysid & 0xff;

	/* We're obviously one of the DEC machines */
	mips_machgroup = MACH_GROUP_DEC;

	switch (dec_systype) {
	case DS2100_3100:
		mips_machtype = MACH_DS23100;
		prom_init_kn01();
		break;
	case DS5100:		/* DS5100 MIPSMATE */
		mips_machtype = MACH_DS5100;
		prom_init_kn230();
		break;
	case DS5000_200:	/* DS5000 3max */
		mips_machtype = MACH_DS5000_200;
		prom_init_kn02();
		break;
	case DS5000_1XX:	/* DS5000/100 3min */
		mips_machtype = MACH_DS5000_1XX;
		prom_init_kn02ba();
		break;
	case DS5000_2X0:	/* DS5000/240 3max+ or DS5900 bigmax */
		mips_machtype = MACH_DS5000_2X0;
		prom_init_kn03();
		if (!(ioasic_read(SIR) & KN03_IO_INR_3MAXP))
			mips_machtype = MACH_DS5900;
		break;
	case DS5000_XX:	/* Personal DS5000/2x */
		mips_machtype = MACH_DS5000_XX;
		prom_init_kn02ca();
		break;
	case DS5800:		/* DS5800 Isis */
		mips_machtype = MACH_DS5800;
		break;
	case DS5400:		/* DS5400 MIPSfair */
		mips_machtype = MACH_DS5400;
		break;
	case DS5500:		/* DS5500 MIPSfair-2 */
		mips_machtype = MACH_DS5500;
		break;
	default:
		mips_machtype = MACH_DSUNKNOWN;
		break;
	}

	if (mips_machtype == MACH_DSUNKNOWN)
		prom_printf("This is an %s, id is %x\n",
			    dec_system_strings[mips_machtype],
			    dec_systype);
	else
		prom_printf("This is a %s\n",
			    dec_system_strings[mips_machtype]);
}
