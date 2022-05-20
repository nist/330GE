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
/* $Id: ioctl.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC_IOCTL_H
#define _SPARC_IOCTL_H

#define _IOC_NRBITS      8
#define _IOC_TYPEBITS    8
#define _IOC_SIZEBITS    8
#define _IOC_RESVBITS    5
#define _IOC_DIRBITS     3

#define _IOC_NRMASK      ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK    ((1 << _IOC_TYPEBITS)-1)
#define _IOC_RESVMASK    ((1 << _IOC_RESVBITS)-1)
#define _IOC_SIZEMASK    ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK     ((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT     0
#define _IOC_TYPESHIFT   (_IOC_NRSHIFT + _IOC_NRBITS)
#define _IOC_SIZESHIFT   (_IOC_TYPESHIFT + _IOC_TYPEBITS)
#define _IOC_RESVSHIFT   (_IOC_SIZESHIFT + _IOC_SIZEBITS)
#define _IOC_DIRSHIFT    (_IOC_RESVSHIFT + _IOC_RESVBITS)

#define _IOC_NONE        1U
#define _IOC_READ        2U
#define _IOC_WRITE       4U

#define _IOC(dir,type,nr,size) \
        (((dir)  << _IOC_DIRSHIFT) | \
         ((type) << _IOC_TYPESHIFT) | \
         ((nr)   << _IOC_NRSHIFT) | \
         ((size) << _IOC_SIZESHIFT))

#define _IO(type,nr)        _IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)  _IOC(_IOC_READ,(type),(nr),sizeof(size))
#define _IOW(type,nr,size)  _IOC(_IOC_WRITE,(type),(nr),sizeof(size))
#define _IOWR(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))

#define _IOC_DIR(nr)        (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)       (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)         (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)       (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

/* ...and for the PCMCIA... */

#define IOC_IN          (_IOC_WRITE << _IOC_DIRSHIFT)
#define IOC_OUT         (_IOC_READ << _IOC_DIRSHIFT)
#define IOC_INOUT       ((_IOC_WRITE|_IOC_READ) << _IOC_DIRSHIFT)
#define IOCSIZE_MASK    (_IOC_SIZEMASK << _IOC_SIZESHIFT)
#define IOCSIZE_SHIFT   (_IOC_SIZESHIFT)

#endif /* !(_SPARC_IOCTL_H) */
