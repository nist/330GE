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
/* $Id: starfire.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * starfire.c: Starfire/E10000 support.
 *
 * Copyright (C) 1998 David S. Miller (davem@redhat.com)
 * Copyright (C) 2000 Anton Blanchard (anton@samba.org)
 */

#include <linux/kernel.h>
#include <linux/slab.h>

#include <asm/page.h>
#include <asm/oplib.h>
#include <asm/smp.h>
#include <asm/upa.h>
#include <asm/starfire.h>

/*
 * A few places around the kernel check this to see if
 * they need to call us to do things in a Starfire specific
 * way.
 */
int this_is_starfire = 0;

void check_if_starfire(void)
{
	int ssnode = prom_finddevice("/ssp-serial");
	if(ssnode != 0 && ssnode != -1)
		this_is_starfire = 1;
}

void starfire_cpu_setup(void)
{
	if (this_is_starfire) {
/*
 * We do this in starfire_translate and xcall_deliver. When we fix our cpu
 * arrays to support > 64 processors we can use the real upaid instead
 * of the logical cpuid in __cpu_number_map etc, then we can get rid of
 * the translations everywhere. - Anton
 */
	}
}

int starfire_hard_smp_processor_id(void)
{
	return upa_readl(0x1fff40000d0UL);
}

/*
 * Each Starfire board has 32 registers which perform translation
 * and delivery of traditional interrupt packets into the extended
 * Starfire hardware format.  Essentially UPAID's now have 2 more
 * bits than in all previous Sun5 systems.
 */
struct starfire_irqinfo {
	unsigned long imap_slots[32];
	unsigned long tregs[32];
	struct starfire_irqinfo *next;
	int upaid, hwmid;
};

static struct starfire_irqinfo *sflist = NULL;

/* Beam me up Scott(McNeil)y... */
void *starfire_hookup(int upaid)
{
	struct starfire_irqinfo *p;
	unsigned long treg_base, hwmid, i;

	p = kmalloc(sizeof(*p), GFP_KERNEL);
	if(!p) {
		prom_printf("starfire_hookup: No memory, this is insane.\n");
		prom_halt();
	}
	treg_base = 0x100fc000000UL;
	hwmid = ((upaid & 0x3c) << 1) |
		((upaid & 0x40) >> 4) |
		(upaid & 0x3);
	p->hwmid = hwmid;
	treg_base += (hwmid << 33UL);
	treg_base += 0x200UL;
	for(i = 0; i < 32; i++) {
		p->imap_slots[i] = 0UL;
		p->tregs[i] = treg_base + (i * 0x10UL);
		/* Lets play it safe and not overwrite existing mappings */
		if (upa_readl(p->tregs[i]) != 0)
			p->imap_slots[i] = 0xdeadbeaf;
	}
	p->upaid = upaid;
	p->next = sflist;
	sflist = p;

	return (void *) p;
}

unsigned int starfire_translate(unsigned long imap,
				unsigned int upaid)
{
	struct starfire_irqinfo *p;
	unsigned int bus_hwmid;
	unsigned int i;

	bus_hwmid = (((unsigned long)imap) >> 33) & 0x7f;
	for(p = sflist; p != NULL; p = p->next)
		if(p->hwmid == bus_hwmid)
			break;
	if(p == NULL) {
		prom_printf("XFIRE: Cannot find irqinfo for imap %016lx\n",
			    ((unsigned long)imap));
		prom_halt();
	}
	for(i = 0; i < 32; i++) {
		if(p->imap_slots[i] == imap ||
		   p->imap_slots[i] == 0UL)
			break;
	}
	if(i == 32) {
		printk("starfire_translate: Are you kidding me?\n");
		panic("Lucy in the sky....");
	}
	p->imap_slots[i] = imap;

	/* map to real upaid */
	upaid = (((upaid & 0x3c) << 1) |
	       ((upaid & 0x40) >> 4) |
	       (upaid & 0x3));

	upa_writel(upaid, p->tregs[i]);

	return i;
}
