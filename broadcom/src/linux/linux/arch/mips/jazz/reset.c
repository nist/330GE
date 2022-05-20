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
 * Reset a Jazz machine.
 */
#include <linux/sched.h>
#include <asm/jazz.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/reboot.h>
#include <asm/delay.h>
#include <asm/keyboard.h>

static inline void kb_wait(void)
{
	unsigned long start = jiffies;

	do {
		if (! (kbd_read_status() & 0x02))
			return;
	} while (jiffies - start < 50);
}

void jazz_machine_restart(char *command)
{
    while (1) {
	kb_wait ();
	kbd_write_command (0xd1);
	kb_wait ();
	kbd_write_output (0x00);
    }
}

void jazz_machine_halt(void)
{
}

void jazz_machine_power_off(void)
{
	/* Jazz machines don't have a software power switch */
}
