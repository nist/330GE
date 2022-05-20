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
/* ffb.h -- ffb DRM template customization -*- linux-c -*-
 */

#ifndef __FFB_H__
#define __FFB_H__

/* This remains constant for all DRM template files.
 */
#define DRM(x) ffb_##x

/* General customization:
 */
#define __HAVE_KERNEL_CTX_SWITCH	1
#define __HAVE_RELEASE			1
#endif
