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
#ifndef _INCLUDE_GUARD_i82092aa_H_
#define _INCLUDE_GUARD_i82092aa_H_

/* $Id: i82092aa.h,v 1.1.1.1 2007/01/25 12:51:57 jiahao_jhou Exp $ */

/* Debuging defines */
#ifdef NOTRACE
#define enter(x)   printk("Enter: %s, %s line %i\n",x,__FILE__,__LINE__)
#define leave(x)   printk("Leave: %s, %s line %i\n",x,__FILE__,__LINE__)
#define dprintk(fmt, args...) printk(fmt , ## args)
#else
#define enter(x)   do {} while (0)
#define leave(x)   do {} while (0)
#define dprintk(fmt, args...) do {} while (0)
#endif



/* prototypes */

static int  i82092aa_pci_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void i82092aa_pci_remove(struct pci_dev *dev);
static int card_present(int socketno);
static void i82092aa_interrupt(int irq, void *dev, struct pt_regs *regs);




static int i82092aa_get_status(unsigned int sock, u_int *value);
static int i82092aa_get_socket(unsigned int sock, socket_state_t *state);
static int i82092aa_set_socket(unsigned int sock, socket_state_t *state);
static int i82092aa_get_io_map(unsigned int sock, struct pccard_io_map *io);
static int i82092aa_set_io_map(unsigned int sock, struct pccard_io_map *io);
static int i82092aa_get_mem_map(unsigned int sock, struct pccard_mem_map *mem);
static int i82092aa_set_mem_map(unsigned int sock, struct pccard_mem_map *mem);
static int i82092aa_init(unsigned int s);
static int i82092aa_suspend(unsigned int sock);
static int i82092aa_register_callback(unsigned int sock, void (*handler)(void *, unsigned int), void * info);
static int i82092aa_inquire_socket(unsigned int sock, socket_cap_t *cap);
static void i82092aa_proc_setup(unsigned int sock, struct proc_dir_entry *base);

#endif

