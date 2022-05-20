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
#ifndef _PARISC_MMZONE_H
#define _PARISC_MMZONE_H

struct node_map_data {
    pg_data_t pg_data;
    struct page *adj_node_mem_map;
};

extern struct node_map_data node_data[];
extern unsigned char *chunkmap;

#define BADCHUNK                ((unsigned char)0xff)
#define CHUNKSZ                 (256*1024*1024)
#define CHUNKSHIFT              28
#define CHUNKMASK               (~(CHUNKSZ - 1))
#define CHUNKNUM(paddr)         ((paddr) >> CHUNKSHIFT)

#define NODE_DATA(nid)          (&node_data[nid].pg_data)
#define NODE_MEM_MAP(nid)       (NODE_DATA(nid)->node_mem_map)
#define ADJ_NODE_MEM_MAP(nid)   (node_data[nid].adj_node_mem_map)

#define phys_to_page(paddr) \
	(ADJ_NODE_MEM_MAP(chunkmap[CHUNKNUM((paddr))]) \
	+ ((paddr) >> PAGE_SHIFT))

#define virt_to_page(kvaddr) phys_to_page(__pa(kvaddr))

/* This is kind of bogus, need to investigate performance of doing it right */
#define VALID_PAGE(page)	((page - mem_map) < max_mapnr)

#endif /* !_PARISC_MMZONE_H */
