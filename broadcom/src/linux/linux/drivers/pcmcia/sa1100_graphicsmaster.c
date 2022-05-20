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
 * drivers/pcmcia/sa1100_graphicsmaster.c
 *
 * PCMCIA implementation routines for GraphicsMaster
 *
 * 9/18/01 Woojung
 *         Fixed wrong PCMCIA voltage setting
 * 7/5/01 Woojung Huh <whuh@applieddata.net>
 *
 */
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/hardware.h>

#include "sa1100_generic.h"
#include "sa1111_generic.h"

static int graphicsmaster_pcmcia_init(struct pcmcia_init *init)
{
  int return_val=0;

  /* Set GPIO_A<3:0> to be outputs for PCMCIA/CF power controller: */
  PA_DDR &= ~(GPIO_GPIO0 | GPIO_GPIO1 | GPIO_GPIO2 | GPIO_GPIO3);

  /* Disable Power 3.3V/5V for PCMCIA/CF */
  PA_DWR |= GPIO_GPIO0 | GPIO_GPIO1 | GPIO_GPIO2 | GPIO_GPIO3;

  /* why? */
  MECR = 0x09430943;

  return sa1111_pcmcia_init(init);
}

static int
graphicsmaster_pcmcia_configure_socket(const struct pcmcia_configure *conf)
{
  unsigned int pa_dwr_mask, pa_dwr_set;
  int ret;

  switch (conf->sock) {
  case 0:
    pa_dwr_mask = GPIO_GPIO0 | GPIO_GPIO1;

    switch (conf->vcc) {
    default:
    case 0:	pa_dwr_set = GPIO_GPIO0 | GPIO_GPIO1;	break;
    case 33:	pa_dwr_set = GPIO_GPIO1;		break;
    case 50:	pa_dwr_set = GPIO_GPIO0;		break;
    }
    break;

  case 1:
    pa_dwr_mask = GPIO_GPIO2 | GPIO_GPIO3;

    switch (conf->vcc) {
    default:
    case 0:	pa_dwr_set = GPIO_GPIO2 | GPIO_GPIO3;	break;
    case 33:	pa_dwr_set = GPIO_GPIO3;		break;
    case 50:	pa_dwr_set = GPIO_GPIO2;		break;
    }
  }

  if (conf->vpp != conf->vcc && conf->vpp != 0) {
    printk(KERN_ERR "%s(): CF slot cannot support Vpp %u\n", __FUNCTION__,
	   conf->vpp);
    return -1;
  }

  ret = sa1111_pcmcia_configure_socket(conf);
  if (ret == 0) {
    unsigned long flags;

    local_irq_save(flags);
    PA_DWR = (PA_DWR & ~pa_dwr_mask) | pa_dwr_set;
    local_irq_restore(flags);
  }

  return ret;
}

struct pcmcia_low_level graphicsmaster_pcmcia_ops = {
  init:			graphicsmaster_pcmcia_init,
  shutdown:		sa1111_pcmcia_shutdown,
  socket_state:		sa1111_pcmcia_socket_state,
  get_irq_info:		sa1111_pcmcia_get_irq_info,
  configure_socket:	graphicsmaster_pcmcia_configure_socket,

  socket_init:		sa1111_pcmcia_socket_init,
  socket_suspend:	sa1111_pcmcia_socket_suspend,
};

