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

/***************************************************************************
 *
 *  drivers/s390/char/tape3490.h
 *    tape device discipline for 3490E tapes.
 *
 *  S390 and zSeries version
 *    Copyright (C) 2001 IBM Corporation
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 *               Tuan Ngo-Anh <ngoanh@de.ibm.com>
 *
 ****************************************************************************
 */

#ifndef _TAPE3490_H

#define _TAPE3490_H


typedef struct _tape3490_disc_data_t {
    __u8 modeset_byte;
} tape3490_disc_data_t  __attribute__ ((packed, aligned(8)));
tape_discipline_t * tape3490_init (int);
#endif // _TAPE3490_H
