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
/* Driver for Microtech DPCM-USB CompactFlash/SmartMedia reader
 *
 * $Id: dpcm.h,v 1.1.1.1 2007/01/25 12:52:00 jiahao_jhou Exp $
 *
 * DPCM driver v0.1:
 *
 * First release
 *
 * Current development and maintenance by:
 *   (c) 2000 Brian Webb (webbb@earthlink.net)
 *
 * See dpcm.c for more explanation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _MICROTECH_DPCM_USB_H
#define _MICROTECH_DPCM_USB_H

extern int dpcm_transport(Scsi_Cmnd *srb, struct us_data *us);

#endif
