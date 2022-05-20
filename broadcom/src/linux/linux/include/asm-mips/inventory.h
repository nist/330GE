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
 * Miguel de Icaza
 */
#ifndef __ASM_INVENTORY_H
#define __ASM_INVENTORY_H

typedef struct inventory_s {
	struct inventory_s *inv_next;
	int    inv_class;
	int    inv_type;
	int    inv_controller;
	int    inv_unit;
	int    inv_state;
} inventory_t;

extern int inventory_items;
void add_to_inventory (int class, int type, int controller, int unit, int state);
int dump_inventory_to_user (void *userbuf, int size);

#endif /* __ASM_INVENTORY_H */
