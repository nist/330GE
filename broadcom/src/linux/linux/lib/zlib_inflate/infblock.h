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
/* infblock.h -- header to use infblock.c
 * Copyright (C) 1995-1998 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

#ifndef _INFBLOCK_H
#define _INFBLOCK_H

struct inflate_blocks_state;
typedef struct inflate_blocks_state FAR inflate_blocks_statef;

extern inflate_blocks_statef * zlib_inflate_blocks_new OF((
    z_streamp z,
    check_func c,               /* check function */
    uInt w));                   /* window size */

extern int zlib_inflate_blocks OF((
    inflate_blocks_statef *,
    z_streamp ,
    int));                      /* initial return code */

extern void zlib_inflate_blocks_reset OF((
    inflate_blocks_statef *,
    z_streamp ,
    uLongf *));                  /* check value on output */

extern int zlib_inflate_blocks_free OF((
    inflate_blocks_statef *,
    z_streamp));

extern void zlib_inflate_set_dictionary OF((
    inflate_blocks_statef *s,
    const Bytef *d,  /* dictionary */
    uInt  n));       /* dictionary length */

extern int zlib_inflate_blocks_sync_point OF((
    inflate_blocks_statef *s));

#endif /* _INFBLOCK_H */
