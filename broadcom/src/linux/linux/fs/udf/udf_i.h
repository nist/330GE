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
#ifndef __LINUX_UDF_I_H
#define __LINUX_UDF_I_H

#define UDF_I(X)			(&((X)->u.udf_i))

#define UDF_I_LOCATION(X)	( UDF_I(X)->i_location )
#define UDF_I_LENEATTR(X)	( UDF_I(X)->i_lenEAttr )
#define UDF_I_LENALLOC(X)	( UDF_I(X)->i_lenAlloc )
#define UDF_I_LENEXTENTS(X)	( UDF_I(X)->i_lenExtents )
#define UDF_I_UNIQUE(X)		( UDF_I(X)->i_unique )
#define UDF_I_ALLOCTYPE(X)	( UDF_I(X)->i_alloc_type )
#define UDF_I_EXTENDED_FE(X)	( UDF_I(X)->i_extended_fe )
#define UDF_I_STRAT4096(X)	( UDF_I(X)->i_strat_4096 )
#define UDF_I_NEW_INODE(X)	( UDF_I(X)->i_new_inode )
#define UDF_I_NEXT_ALLOC_BLOCK(X)	( UDF_I(X)->i_next_alloc_block )
#define UDF_I_NEXT_ALLOC_GOAL(X)	( UDF_I(X)->i_next_alloc_goal )
#define UDF_I_UMTIME(X)		( UDF_I(X)->i_umtime )
#define UDF_I_UCTIME(X)		( UDF_I(X)->i_uctime )
#define UDF_I_CRTIME(X)		( UDF_I(X)->i_crtime )
#define UDF_I_UCRTIME(X)	( UDF_I(X)->i_ucrtime )

#endif /* !defined(_LINUX_UDF_I_H) */
