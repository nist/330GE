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
/*
 *  arch/s390/kernel/ieee.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 */

#include <linux/sched.h>

static    inline void _adddf(int R1,int R2)
{
  current->tss.fprs[R1].fd = current->tss.fprs[R1].fd +
                             current->tss.fprs[R2].fd;
}

static    inline void _subdf(int R1,int R2)
{
  current->tss.fprs[R1].fd = current->tss.fprs[R1].fd -
                             current->tss.fprs[R2].fd;
}

static    inline void _muldf(int R1,int R2)
{
  current->tss.fprs[R1].fd = current->tss.fprs[R1].fd *
                             current->tss.fprs[R2].fd;
}

static    inline void _divdf(int R1,int R2)
{
  current->tss.fprs[R1].fd = current->tss.fprs[R1].fd /
                             current->tss.fprs[R2].fd;
}

static    inline void _negdf(int R1,int R2)
{
  current->tss.fprs[R1].fd = -current->tss.fprs[R1].fd;
}

static    inline void _fixdfsi(int R1,int R2)
{
  current->tss.regs->gprs[R1] = (__u32) current->tss.fprs[R2].fd;
}

static    inline void _extendsidf(int R1,int R2)
{
  current->tss.fprs[R1].fd = (double) current->tss.regs->gprs[R2];
}


static    inline  void _addsf(int R1,int R2)
{
  current->tss.fprs[R1].ff = current->tss.fprs[R1].ff +
                             current->tss.fprs[R2].ff;
}

static    inline  void _subsf(int R1,int R2)
{
  current->tss.fprs[R1].ff = current->tss.fprs[R1].ff -
                             current->tss.fprs[R2].ff;
}

static    inline void _mulsf(int R1,int R2)
{
  current->tss.fprs[R1].ff = current->tss.fprs[R1].ff *
                             current->tss.fprs[R2].ff;
}

static    inline void _divsf(int R1,int R2)
{
  current->tss.fprs[R1].ff = current->tss.fprs[R1].ff /
                             current->tss.fprs[R2].ff;
}

static    inline void _negsf(int R1,int R2)
{
  current->tss.fprs[R1].ff = -current->tss.fprs[R1].ff;
}

static    inline void _fixsfsi(int R1,int R2)
{
  current->tss.regs->gprs[R1] = (__u32) current->tss.fprs[R2].ff;
}

static    inline void _extendsisf(int R1,int R2)
{
  current->tss.fprs[R1].ff = (double) current->tss.regs->gprs[R2];
}


