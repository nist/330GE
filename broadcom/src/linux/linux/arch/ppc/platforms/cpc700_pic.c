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
 * arch/ppc/platforms/cpc700_pic.c
 * 
 * Interrupt controller support for IBM Spruce
 *
 * Authors: Mark Greer, Matt Porter, and Johnnie Peters
 *	    mgreer@mvista.com
 *          mporter@mvista.com
 *          jpeters@mvista.com
 *
 * Copyright 2001-2002 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR   IMPLIED
 * WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT,  INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 * USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/irq.h>

#include <asm/io.h>
#include <asm/processor.h>
#include <asm/system.h>
#include <asm/irq.h>

#include "cpc700.h"

static void
cpc700_unmask_irq(unsigned int irq)
{
	unsigned int tr_bits;

	/*
	 * IRQ 31 is largest IRQ supported.
	 * IRQs 17-19 are reserved.
	 */
	if ((irq <= 31) && ((irq < 17) || (irq > 19))) {
		tr_bits = CPC700_IN_32(CPC700_UIC_UICTR);

		if ((tr_bits & (1 << (31 - irq))) == 0) {
			/* level trigger interrupt, clear bit in status
			 * register */
			CPC700_OUT_32(CPC700_UIC_UICSR, 1 << (31 - irq));
		}

		/* Know IRQ fits in entry 0 of ppc_cached_irq_mask[] */
		ppc_cached_irq_mask[0] |= CPC700_UIC_IRQ_BIT(irq);
		
		CPC700_OUT_32(CPC700_UIC_UICER, ppc_cached_irq_mask[0]);
	}
	return;
}

static void
cpc700_mask_irq(unsigned int irq)
{
	/*
	 * IRQ 31 is largest IRQ supported.
	 * IRQs 17-19 are reserved.
	 */
	if ((irq <= 31) && ((irq < 17) || (irq > 19))) {
		/* Know IRQ fits in entry 0 of ppc_cached_irq_mask[] */
		ppc_cached_irq_mask[0] &=
			~CPC700_UIC_IRQ_BIT(irq);

		CPC700_OUT_32(CPC700_UIC_UICER, ppc_cached_irq_mask[0]);
	}
	return;
}

static void
cpc700_mask_and_ack_irq(unsigned int irq)
{
	u_int	bit;

	/*
	 * IRQ 31 is largest IRQ supported.
	 * IRQs 17-19 are reserved.
	 */
	if ((irq <= 31) && ((irq < 17) || (irq > 19))) {
		/* Know IRQ fits in entry 0 of ppc_cached_irq_mask[] */
		bit = CPC700_UIC_IRQ_BIT(irq);

		ppc_cached_irq_mask[0] &= ~bit;
		CPC700_OUT_32(CPC700_UIC_UICER, ppc_cached_irq_mask[0]);
		CPC700_OUT_32(CPC700_UIC_UICSR, bit); /* Write 1 clears IRQ */
	}
	return;
}

static struct hw_interrupt_type cpc700_pic = {
	"CPC700 PIC",
	NULL,
	NULL,
	cpc700_unmask_irq,
	cpc700_mask_irq,
	cpc700_mask_and_ack_irq,
	NULL,
	NULL
};

__init static void
cpc700_pic_init_irq(unsigned int irq)
{
	unsigned int tmp;

	/* Set interrupt sense */
	tmp = CPC700_IN_32(CPC700_UIC_UICTR);
	if (cpc700_irq_assigns[irq][0] == 0) {
		tmp &= ~CPC700_UIC_IRQ_BIT(irq);
	} else {
		tmp |= CPC700_UIC_IRQ_BIT(irq);
	}
	CPC700_OUT_32(CPC700_UIC_UICTR, tmp);

	/* Set interrupt polarity */
	tmp = CPC700_IN_32(CPC700_UIC_UICPR);
	if (cpc700_irq_assigns[irq][1]) {
		tmp |= CPC700_UIC_IRQ_BIT(irq);
	} else {
		tmp &= ~CPC700_UIC_IRQ_BIT(irq);
	}
	CPC700_OUT_32(CPC700_UIC_UICPR, tmp);

	/* Set interrupt critical */
	tmp = CPC700_IN_32(CPC700_UIC_UICCR);
	tmp |= CPC700_UIC_IRQ_BIT(irq);
	CPC700_OUT_32(CPC700_UIC_UICCR, tmp);
			
	return;
}
	
__init void
cpc700_init_IRQ(void)
{
	int i;

	ppc_cached_irq_mask[0] = 0;
	CPC700_OUT_32(CPC700_UIC_UICER, 0x00000000);    /* Disable all irq's */
	CPC700_OUT_32(CPC700_UIC_UICSR, 0xffffffff);    /* Clear cur intrs */
	CPC700_OUT_32(CPC700_UIC_UICCR, 0xffffffff);    /* Gen INT not MCP */
	CPC700_OUT_32(CPC700_UIC_UICPR, 0x00000000);    /* Active low */
	CPC700_OUT_32(CPC700_UIC_UICTR, 0x00000000);    /* Level Sensitive */
	CPC700_OUT_32(CPC700_UIC_UICVR, CPC700_UIC_UICVCR_0_HI);
						        /* IRQ 0 is highest */

	for (i = 0; i < 17; i++) {
		irq_desc[i].handler = &cpc700_pic;
		cpc700_pic_init_irq(i);
	}

	for (i = 20; i < 32; i++) {
		irq_desc[i].handler = &cpc700_pic;
		cpc700_pic_init_irq(i);
	}

	return;
}



/*
 * Find the highest IRQ that generating an interrupt, if any.
 */
int
cpc700_get_irq(struct pt_regs *regs)
{
	int irq = 0;
	u_int irq_status, irq_test = 1;

	irq_status = CPC700_IN_32(CPC700_UIC_UICMSR);

	do
	{
		if (irq_status & irq_test)
			break;
		irq++;
		irq_test <<= 1;
	} while (irq < NR_IRQS);
		

	if (irq == NR_IRQS)
	    irq = 33;

	return (31 - irq);
}