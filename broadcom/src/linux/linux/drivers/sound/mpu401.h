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

/*	From uart401.c */
int probe_uart401 (struct address_info *hw_config, struct module *owner);
void unload_uart401 (struct address_info *hw_config);

void uart401intr (int irq, void *dev_id, struct pt_regs * dummy);

/*	From mpu401.c */
int probe_mpu401(struct address_info *hw_config);
int attach_mpu401(struct address_info * hw_config, struct module *owner);
void unload_mpu401(struct address_info *hw_info);

int intchk_mpu401(void *dev_id);
void mpuintr(int irq, void *dev_id, struct pt_regs * dummy);
