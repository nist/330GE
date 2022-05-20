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
/* RS-485 structures */

/* RS-485 support */
/* Used with ioctl() TIOCSERSETRS485 */
struct rs485_control {
        unsigned short rts_on_send;
        unsigned short rts_after_sent;
        __u32 delay_rts_before_send;
        unsigned short enabled;
#ifdef __KERNEL__
        int disable_serial_loopback;
#endif
};

/* Used with ioctl() TIOCSERWRRS485 */
struct rs485_write {
        __u16 outc_size;
        unsigned char *outc;
};

