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
#include <linux/types.h>

void * memcpy(void * to, const void * from, size_t n)
{
  void *xto = to;
  size_t temp, temp1;

  if (!n)
    return xto;
  if ((long) to & 1)
    {
      char *cto = to;
      const char *cfrom = from;
      *cto++ = *cfrom++;
      to = cto;
      from = cfrom;
      n--;
    }
  if (n > 2 && (long) to & 2)
    {
      short *sto = to;
      const short *sfrom = from;
      *sto++ = *sfrom++;
      to = sto;
      from = sfrom;
      n -= 2;
    }
  temp = n >> 2;
  if (temp)
    {
      long *lto = to;
      const long *lfrom = from;

      __asm__ __volatile__("movel %2,%3\n\t"
			   "andw  #7,%3\n\t"
			   "lsrl  #3,%2\n\t"
			   "negw  %3\n\t"
			   "jmp   %%pc@(1f,%3:w:2)\n\t"
			   "4:\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "movel %0@+,%1@+\n\t"
			   "1:\t"
			   "dbra  %2,4b\n\t"
			   "clrw  %2\n\t"
			   "subql #1,%2\n\t"
			   "jpl   4b\n\t"
			   : "=a" (lfrom), "=a" (lto), "=d" (temp),
			   "=&d" (temp1)
			   : "0" (lfrom), "1" (lto), "2" (temp)
			   );
      to = lto;
      from = lfrom;
    }
  if (n & 2)
    {
      short *sto = to;
      const short *sfrom = from;
      *sto++ = *sfrom++;
      to = sto;
      from = sfrom;
    }
  if (n & 1)
    {
      char *cto = to;
      const char *cfrom = from;
      *cto = *cfrom;
    }
  return xto;
}
