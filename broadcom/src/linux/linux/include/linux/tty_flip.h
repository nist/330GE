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
#ifndef _LINUX_TTY_FLIP_H
#define _LINUX_TTY_FLIP_H

#ifdef INCLUDE_INLINE_FUNCS
#define _INLINE_ extern
#else
#define _INLINE_ static __inline__
#endif

_INLINE_ void tty_insert_flip_char(struct tty_struct *tty,
				   unsigned char ch, char flag)
{
	if (tty->flip.count < TTY_FLIPBUF_SIZE) {
		tty->flip.count++;
		*tty->flip.flag_buf_ptr++ = flag;
		*tty->flip.char_buf_ptr++ = ch;
	}
}

_INLINE_ void tty_schedule_flip(struct tty_struct *tty)
{
	queue_task(&tty->flip.tqueue, &tq_timer);
}

#undef _INLINE_


#endif /* _LINUX_TTY_FLIP_H */







