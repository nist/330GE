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
#include <linux/config.h>
#include <linux/kd.h>

int  (*k_setkeycode)(unsigned int, unsigned int);
int  (*k_getkeycode)(unsigned int);
int  (*k_translate)(unsigned char, unsigned char *, char);
char (*k_unexpected_up)(unsigned char);
void (*k_leds)(unsigned char);

#ifdef CONFIG_MAGIC_SYSRQ
int k_sysrq_key;
unsigned char *k_sysrq_xlate;
#endif
