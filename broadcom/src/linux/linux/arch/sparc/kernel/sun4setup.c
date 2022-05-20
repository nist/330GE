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
/* sun4setup.c: Setup the hardware address of various items in the sun4
 * 		architecture. Called from idprom_init
 *
 * Copyright (C) 1998 Chris G. Davis (cdavis@cois.on.ca)
 */

#include <asm/page.h>
#include <asm/oplib.h>
#include <asm/idprom.h>
#include <asm/sun4paddr.h>
#include <asm/machines.h>

int sun4_memreg_physaddr;
int sun4_ie_physaddr;
int sun4_clock_physaddr;
int sun4_timer_physaddr;
int sun4_eth_physaddr;
int sun4_si_physaddr;
int sun4_bwtwo_physaddr;
int sun4_zs0_physaddr;
int sun4_zs1_physaddr;
int sun4_dma_physaddr;
int sun4_esp_physaddr;
int sun4_ie_physaddr; 

void __init sun4setup(void)
{
	printk("Sun4 Hardware Setup v1.0 18/May/98 Chris Davis (cdavis@cois.on.ca). ");
	/*
	  setup standard sun4 info
	  */
	sun4_ie_physaddr=SUN4_IE_PHYSADDR;

	/*
	  setup model specific info
	  */
	switch(idprom->id_machtype) {
		case (SM_SUN4 | SM_4_260 ):
			printk("Setup for a SUN4/260\n");
			sun4_memreg_physaddr=SUN4_200_MEMREG_PHYSADDR;
			sun4_clock_physaddr=SUN4_200_CLOCK_PHYSADDR;
			sun4_timer_physaddr=SUN4_UNUSED_PHYSADDR;
			sun4_eth_physaddr=SUN4_200_ETH_PHYSADDR;
			sun4_si_physaddr=SUN4_200_SI_PHYSADDR;
			sun4_bwtwo_physaddr=SUN4_200_BWTWO_PHYSADDR;
			sun4_dma_physaddr=SUN4_UNUSED_PHYSADDR;
			sun4_esp_physaddr=SUN4_UNUSED_PHYSADDR;
			break;
		case (SM_SUN4 | SM_4_330 ):
			printk("Setup for a SUN4/330\n");
			sun4_memreg_physaddr=SUN4_300_MEMREG_PHYSADDR;
			sun4_clock_physaddr=SUN4_300_CLOCK_PHYSADDR;
			sun4_timer_physaddr=SUN4_300_TIMER_PHYSADDR;
			sun4_eth_physaddr=SUN4_300_ETH_PHYSADDR;
			sun4_si_physaddr=SUN4_UNUSED_PHYSADDR;
			sun4_bwtwo_physaddr=SUN4_300_BWTWO_PHYSADDR;
			sun4_dma_physaddr=SUN4_300_DMA_PHYSADDR;
			sun4_esp_physaddr=SUN4_300_ESP_PHYSADDR;
			break;
		case (SM_SUN4 | SM_4_470 ):
			printk("Setup for a SUN4/470\n");
			sun4_memreg_physaddr=SUN4_400_MEMREG_PHYSADDR;
			sun4_clock_physaddr=SUN4_400_CLOCK_PHYSADDR;
			sun4_timer_physaddr=SUN4_400_TIMER_PHYSADDR;
			sun4_eth_physaddr=SUN4_400_ETH_PHYSADDR;
			sun4_si_physaddr=SUN4_UNUSED_PHYSADDR;
			sun4_bwtwo_physaddr=SUN4_400_BWTWO_PHYSADDR;
			sun4_dma_physaddr=SUN4_400_DMA_PHYSADDR;
			sun4_esp_physaddr=SUN4_400_ESP_PHYSADDR;
			break;
		default:
	}
}

