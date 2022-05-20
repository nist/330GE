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
#ifndef _ASMS390_SCATTERLIST_H
#define _ASMS390_SCATTERLIST_H

struct scatterlist {
    /* This will disappear in 2.5.x */
    char *address;

    /* These two are only valid if ADDRESS member of this
     * struct is NULL.
     */
    struct page *page;
    unsigned int offset;

    unsigned int length;
};

#define ISA_DMA_THRESHOLD (0xffffffffffffffff)

#endif /* _ASMS390X_SCATTERLIST_H */
