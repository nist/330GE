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
/* gcclib.h -- definitions for various functions 'borrowed' from gcc-2.95.3 */
/* I Molton     29/07/01 */

#define BITS_PER_UNIT  8
#define SI_TYPE_SIZE (sizeof (SItype) * BITS_PER_UNIT)

typedef unsigned int UQItype    __attribute__ ((mode (QI)));
typedef          int SItype     __attribute__ ((mode (SI)));
typedef unsigned int USItype    __attribute__ ((mode (SI)));
typedef          int DItype     __attribute__ ((mode (DI)));
typedef          int word_type 	__attribute__ ((mode (__word__)));
typedef unsigned int UDItype    __attribute__ ((mode (DI)));

#ifdef __ARMEB__
  struct DIstruct {SItype high, low;};
#else
  struct DIstruct {SItype low, high;};
#endif

typedef union
{
  struct DIstruct s;
  DItype ll;
} DIunion;

