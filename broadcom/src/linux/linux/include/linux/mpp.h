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
#ifndef _LINUX_MPP_H
#define _LINUX_MPP_H

/*
 * Definitions related to Massively Parallel Processing support.
 */

/* All mpp implementations must supply these functions */

extern void mpp_init(void);
extern void mpp_hw_init(void);
extern void mpp_procfs_init(void);

extern int mpp_num_cells(void);
extern int mpp_cid(void);
extern int get_mppinfo(char *buffer);

#endif
