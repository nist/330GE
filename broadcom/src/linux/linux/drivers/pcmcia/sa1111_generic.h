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
extern int sa1111_pcmcia_init(struct pcmcia_init *);
extern int sa1111_pcmcia_shutdown(void);
extern int sa1111_pcmcia_socket_state(struct pcmcia_state_array *);
extern int sa1111_pcmcia_get_irq_info(struct pcmcia_irq_info *);
extern int sa1111_pcmcia_configure_socket(const struct pcmcia_configure *);
extern int sa1111_pcmcia_socket_init(int);
extern int sa1111_pcmcia_socket_suspend(int);

/*
 * I'd really like to move the INTPOL stuff to arch/arm/mach-sa1100/sa1111.c
 */
#define SA1111_IRQMASK_LO(x)	(1 << (x - IRQ_SA1111_START))
#define SA1111_IRQMASK_HI(x)	(1 << (x - IRQ_SA1111_START - 32))


