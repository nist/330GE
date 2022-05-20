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
#ifndef __ASM_SH_BIOS_H
#define __ASM_SH_BIOS_H

/*
 * Copyright (C) 2000 Greg Banks, Mitch Davis
 * C API to interface to the standard LinuxSH BIOS
 * usually from within the early stages of kernel boot.
 */


extern void sh_bios_console_write(const char *buf, unsigned int len);
extern void sh_bios_char_out(char ch);
extern int sh_bios_in_gdb_mode(void);
extern void sh_bios_gdb_detach(void);

extern void sh_bios_get_node_addr(unsigned char *node_addr);
extern void sh_bios_shutdown(unsigned int how);

#endif /* __ASM_SH_BIOS_H */
