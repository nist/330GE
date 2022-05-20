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

#ifndef	__ISERIES_IRQ_H__
#define	__ISERIES_IRQ_H__


#ifdef __cplusplus
extern "C" {
#endif

unsigned int iSeries_startup_IRQ(unsigned int);
void iSeries_shutdown_IRQ(unsigned int);
void iSeries_enable_IRQ(unsigned int);
void iSeries_disable_IRQ(unsigned int);
void iSeries_end_IRQ(unsigned int);
void iSeries_init_IRQ(void);
void iSeries_init_irqMap(int);
int  iSeries_allocate_IRQ(HvBusNumber, HvSubBusNumber, HvAgentId);
int  iSeries_assign_IRQ(int, HvBusNumber, HvSubBusNumber, HvAgentId);
void iSeries_activate_IRQs(void);

int XmPciLpEvent_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __ISERIES_IRQ_H__ */
