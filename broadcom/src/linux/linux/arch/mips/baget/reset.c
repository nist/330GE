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
#include <linux/kernel.h>
#include <asm/system.h>
#include <asm/baget/baget.h>


#define R3000_RESET_VEC  0xbfc00000
typedef void vector(void);


static void baget_reboot(char *from_fun)
{
	cli();
	baget_printk("\n%s: jumping to RESET code...\n", from_fun);
	(*(vector*)R3000_RESET_VEC)();
}


void baget_machine_restart(char *command)
{
	baget_reboot("restart");
}

void baget_machine_halt(void)
{
	baget_reboot("halt");
}

void baget_machine_power_off(void)
{
	baget_reboot("power off");
}
