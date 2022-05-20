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
 *  linux/arch/mips/sni/process.c
 *
 *  Reset a SNI machine.
 */
#include <asm/io.h>
#include <asm/reboot.h>
#include <asm/system.h>
#include <asm/sni.h>

/*
 * This routine reboots the machine by asking the keyboard
 * controller to pulse the reset-line low. We try that for a while,
 * and if it doesn't work, we do some other stupid things.
 */
static inline void
kb_wait(void)
{
	int i;

	for (i=0; i<0x10000; i++)
		if ((inb_p(0x64) & 0x02) == 0)
			break;
}

void sni_machine_restart(char *command)
{
	int i, j;

	/* This does a normal via the keyboard controller like a PC.
	   We can do that easier ...  */
	sti();
	for (;;) {
		for (i=0; i<100; i++) {
			kb_wait();
			for(j = 0; j < 100000 ; j++)
				/* nothing */;
			outb_p(0xfe,0x64);	 /* pulse reset low */
		}
	}
}

void sni_machine_halt(void)
{
}

void sni_machine_power_off(void)
{
	*(volatile unsigned char *)PCIMT_CSWCSM = 0xfd;
}
