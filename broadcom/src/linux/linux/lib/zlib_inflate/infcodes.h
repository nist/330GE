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
/* infcodes.h -- header to use infcodes.c
 * Copyright (C) 1995-1998 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

#ifndef _INFCODES_H
#define _INFCODES_H

#include "infblock.h"

struct inflate_codes_state;
typedef struct inflate_codes_state FAR inflate_codes_statef;

extern inflate_codes_statef *zlib_inflate_codes_new OF((
    uInt, uInt,
    inflate_huft *, inflate_huft *,
    z_streamp ));

extern int zlib_inflate_codes OF((
    inflate_blocks_statef *,
    z_streamp ,
    int));

extern void zlib_inflate_codes_free OF((
    inflate_codes_statef *,
    z_streamp ));

#endif /* _INFCODES_H */
