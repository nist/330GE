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
/* $Id: pil.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_PIL_H
#define _SPARC64_PIL_H

/* To avoid some locking problems, we hard allocate certain PILs
 * for SMP cross call messages that must do a etrap/rtrap.
 *
 * A cli() does not block the cross call delivery, so when SMP
 * locking is an issue we reschedule the event into a PIL interrupt
 * which is blocked by cli().
 *
 * In fact any XCALL which has to etrap/rtrap has a problem because
 * it is difficult to prevent rtrap from running BH's, and that would
 * need to be done if the XCALL arrived while %pil==15.
 */
#define PIL_SMP_CALL_FUNC	1
#define PIL_SMP_RECEIVE_SIGNAL	2
#define PIL_SMP_CAPTURE		3

#ifndef __ASSEMBLY__
#define PIL_RESERVED(PIL)	((PIL) == PIL_SMP_CALL_FUNC || \
				 (PIL) == PIL_SMP_RECEIVE_SIGNAL || \
				 (PIL) == PIL_SMP_CAPTURE)
#endif

#endif /* !(_SPARC64_PIL_H) */
