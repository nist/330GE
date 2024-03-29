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
/* Low-level parallel port routines for the Amiga built-in port
 *
 * Author: Joerg Dorchain <joerg@dorchain.net>
 *
 * This is a complete rewrite of the code, but based heaviy upon the old
 * lp_intern. code.
 *
 * The built-in Amiga parallel port provides one port at a fixed address
 * with 8 bidirectional data lines (D0 - D7) and 3 bidirectional status
 * lines (BUSY, POUT, SEL), 1 output control line /STROBE (raised automatically
 * in hardware when the data register is accessed), and 1 input control line
 * /ACK, able to cause an interrupt, but both not directly settable by
 * software.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/parport.h>
#include <linux/ioport.h>
#include <asm/setup.h>
#include <asm/amigahw.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/amigaints.h>

#undef DEBUG
#ifdef DEBUG
#define DPRINTK printk
#else
#define DPRINTK(x...)	do { } while (0)
#endif

static struct parport *this_port = NULL;

static void amiga_write_data(struct parport *p, unsigned char data)
{
	DPRINTK(KERN_DEBUG "write_data %c\n",data);
	/* Triggers also /STROBE. This behavior cannot be changed */
	ciaa.prb = data;
	mb();
}

static unsigned char amiga_read_data(struct parport *p)
{
	/* Triggers also /STROBE. This behavior cannot be changed */
	return ciaa.prb;
}


static unsigned char control_amiga_to_pc(unsigned char control)
{
	return PARPORT_CONTROL_SELECT |
	      PARPORT_CONTROL_AUTOFD | PARPORT_CONTROL_STROBE;
	/* fake value: interrupt enable, select in, no reset,
	no autolf, no strobe - seems to be closest the wiring diagram */
}

static void amiga_write_control(struct parport *p, unsigned char control)
{
	DPRINTK(KERN_DEBUG "write_control %02x\n",control);
	/* No implementation possible */
}
	
static unsigned char amiga_read_control( struct parport *p)
{
	DPRINTK(KERN_DEBUG "read_control \n");
	return control_amiga_to_pc(0);
}

static unsigned char amiga_frob_control( struct parport *p, unsigned char mask, unsigned char val)
{
	unsigned char old;

	DPRINTK(KERN_DEBUG "frob_control mask %02x, value %02x\n",mask,val);
	old = amiga_read_control(p);
	amiga_write_control(p, (old & ~mask) ^ val);
	return old;
}


static unsigned char status_amiga_to_pc(unsigned char status)
{
	unsigned char ret = PARPORT_STATUS_BUSY | PARPORT_STATUS_ACK | PARPORT_STATUS_ERROR;

	if (status & 1) /* Busy */
		ret &= ~PARPORT_STATUS_BUSY;
	if (status & 2) /* PaperOut */
		ret |= PARPORT_STATUS_PAPEROUT;
	if (status & 4) /* Selected */
		ret |= PARPORT_STATUS_SELECT;
	/* the rest is not connected or handled autonomously in hardware */

	return ret;
}

static unsigned char amiga_read_status(struct parport *p)
{
	unsigned char status;

	status = status_amiga_to_pc(ciab.pra & 7);
	DPRINTK(KERN_DEBUG "read_status %02x\n", status);
	return status;
}

/* as this ports irq handling is already done, we use a generic funktion */
static void amiga_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	parport_generic_irq(irq, (struct parport *) dev_id, regs);
}

static void amiga_enable_irq(struct parport *p)
{
	enable_irq(IRQ_AMIGA_CIAA_FLG);
}

static void amiga_disable_irq(struct parport *p)
{
	disable_irq(IRQ_AMIGA_CIAA_FLG);
}

static void amiga_data_forward(struct parport *p)
{
	DPRINTK(KERN_DEBUG "forward\n");
	ciaa.ddrb = 0xff; /* all pins output */
	mb();
}

static void amiga_data_reverse(struct parport *p)
{
	DPRINTK(KERN_DEBUG "reverse\n");
	ciaa.ddrb = 0; /* all pins input */
	mb();
}

static void amiga_init_state(struct pardevice *dev, struct parport_state *s)
{
	s->u.amiga.data = 0;
	s->u.amiga.datadir = 255;
	s->u.amiga.status = 0;
	s->u.amiga.statusdir = 0;
}

static void amiga_save_state(struct parport *p, struct parport_state *s)
{
	mb();
	s->u.amiga.data = ciaa.prb;
	s->u.amiga.datadir = ciaa.ddrb;
	s->u.amiga.status = ciab.pra & 7;
	s->u.amiga.statusdir = ciab.ddra & 7;
	mb();
}

static void amiga_restore_state(struct parport *p, struct parport_state *s)
{
	mb();
	ciaa.prb = s->u.amiga.data;
	ciaa.ddrb = s->u.amiga.datadir;
	ciab.pra |= (ciab.pra & 0xf8) | s->u.amiga.status;
	ciab.ddra |= (ciab.ddra & 0xf8) | s->u.amiga.statusdir;
	mb();
}

static void amiga_inc_use_count(void)
{
	MOD_INC_USE_COUNT;
}

static void amiga_dec_use_count(void)
{
	MOD_DEC_USE_COUNT;
}

static struct parport_operations pp_amiga_ops = {
	amiga_write_data,
	amiga_read_data,

	amiga_write_control,
	amiga_read_control,
	amiga_frob_control,

	amiga_read_status,

	amiga_enable_irq,
	amiga_disable_irq,

	amiga_data_forward,
	amiga_data_reverse,

	amiga_init_state,
	amiga_save_state,
	amiga_restore_state,

	amiga_inc_use_count,
	amiga_dec_use_count,

	parport_ieee1284_epp_write_data,
	parport_ieee1284_epp_read_data,
	parport_ieee1284_epp_write_addr,
	parport_ieee1284_epp_read_addr,

	parport_ieee1284_ecp_write_data,
	parport_ieee1284_ecp_read_data,
	parport_ieee1284_ecp_write_addr,

	parport_ieee1284_write_compat,
	parport_ieee1284_read_nibble,
	parport_ieee1284_read_byte,
};

/* ----------- Initialisation code --------------------------------- */

int __init parport_amiga_init(void)
{
	struct parport *p;
	int err;

	if (!MACH_IS_AMIGA || !AMIGAHW_PRESENT(AMI_PARALLEL))
		return -ENODEV;

	err = -EBUSY;
	if (!request_mem_region(CIAA_PHYSADDR-1+0x100, 0x100, "parallel"))
		goto out_mem;

	ciaa.ddrb = 0xff;
	ciab.ddra &= 0xf8;
	mb();

	p = parport_register_port((unsigned long)&ciaa.prb, IRQ_AMIGA_CIAA_FLG,
				   PARPORT_DMA_NONE, &pp_amiga_ops);
	if (!p)
		goto out_port;

	err = request_irq(IRQ_AMIGA_CIAA_FLG, amiga_interrupt, 0, p->name, p);
	if (err)
		goto out_irq;

	this_port = p;
	printk(KERN_INFO "%s: Amiga built-in port using irq\n", p->name);
	parport_proc_register(p);

	parport_announce_port(p);

	return 0;

out_irq:
	parport_unregister_port(p);
out_port:
	release_mem_region(CIAA_PHYSADDR-1+0x100, 0x100);
out_mem:
	return err;
}

void __exit parport_amiga_exit(void)
{
	if (this_port->irq != PARPORT_IRQ_NONE)
		free_irq(IRQ_AMIGA_CIAA_FLG, this_port);
	parport_proc_unregister(this_port);
	parport_unregister_port(this_port);
	release_mem_region(CIAA_PHYSADDR-1+0x100, 0x100);
}


MODULE_AUTHOR("Joerg Dorchain <joerg@dorchain.net>");
MODULE_DESCRIPTION("Parport Driver for Amiga builtin Port");
MODULE_SUPPORTED_DEVICE("Amiga builtin Parallel Port");
MODULE_LICENSE("GPL");

module_init(parport_amiga_init)
module_exit(parport_amiga_exit)
