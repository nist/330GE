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
#warning	"REVISIT_PFS168: Need to verify and test GPIO power encodings."
/*
 * drivers/pcmcia/sa1100_pfs168.c
 *
 * PFS168 PCMCIA specific routines
 *
 */
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/delay.h>
#include <asm/hardware.h>
#include <asm/irq.h>

#include "sa1100_generic.h"
#include "sa1111_generic.h"

static int pfs168_pcmcia_init(struct pcmcia_init *init)
{
  /* TPS2211 to standby mode: */
  PA_DWR &= ~(GPIO_GPIO0 | GPIO_GPIO1 | GPIO_GPIO2 | GPIO_GPIO3);

  /* Set GPIO_A<3:0> to be outputs for PCMCIA (socket 0) power controller: */
  PA_DDR &= ~(GPIO_GPIO0 | GPIO_GPIO1 | GPIO_GPIO2 | GPIO_GPIO3);

  return sa1111_pcmcia_init(init);
}

static int
pfs168_pcmcia_configure_socket(const struct pcmcia_configure *conf)
{
  unsigned int pa_dwr_mask = 0, pa_dwr_set = 0;
  int ret;

  /* PFS168 uses the Texas Instruments TPS2211 for PCMCIA (socket 0) voltage control only,
   * with the following connections:
   *
   *   TPS2211      PFS168
   *
   *    -VCCD0      SA-1111 GPIO A<0>
   *    -VCCD0      SA-1111 GPIO A<1>
   *     VPPD0      SA-1111 GPIO A<2>
   *     VPPD0      SA-1111 GPIO A<2>
   *
   */

  switch (conf->sock) {
  case 0:
    pa_dwr_mask = GPIO_GPIO0 | GPIO_GPIO1 | GPIO_GPIO2 | GPIO_GPIO3;

    switch (conf->vcc) {
    default:
    case 0:	pa_dwr_set = 0;			break;
    case 33:	pa_dwr_set = GPIO_GPIO0;	break;
    case 50:	pa_dwr_set = GPIO_GPIO1;	break;
    }

    switch (conf->vpp) {
    case 0:
      break;

    case 120:
      printk(KERN_ERR "%s(): PFS-168 does not support VPP %uV\n",
	     __FUNCTION__, conf->vpp / 10);
      return -1;
      break;

    default:
      if (conf->vpp == conf->vcc)
        pa_dwr_set |= GPIO_GPIO3;
      else {
	printk(KERN_ERR "%s(): unrecognized VPP %u\n", __FUNCTION__,
	       conf->vpp);
	return -1;
      }
    }
    break;

  case 1:
    pa_dwr_mask = 0;
    pa_dwr_set = 0;

    switch (conf->vcc) {
    case 0:
    case 33:
      break;

    case 50:
      printk(KERN_ERR "%s(): PFS-168 CompactFlash socket does not support VCC %uV\n",
	     __FUNCTION__, conf->vcc / 10);
      return -1;

    default:
      printk(KERN_ERR "%s(): unrecognized VCC %u\n", __FUNCTION__,
	     conf->vcc);
      return -1;
    }

    if (conf->vpp != conf->vcc && conf->vpp != 0) {
      printk(KERN_ERR "%s(): CompactFlash socket does not support VPP %uV\n"
	      __FUNCTION__, conf->vpp/10);
      return -1;
    }
    break;
  }

  ret = sa1111_pcmcia_configure_socket(conf);
  if (ret == 0) {
    unsigned long flags;

    local_irq_save(flags);
    PA_DWR = (PA_DWR & ~pa_dwr_mask) | pa_dwr_set;
    local_irq_restore(flags);
  }

  return 0;
}

struct pcmcia_low_level pfs168_pcmcia_ops = {
  init:			pfs168_pcmcia_init,
  shutdown:		sa1111_pcmcia_shutdown,
  socket_state:		sa1111_pcmcia_socket_state,
  get_irq_info:		sa1111_pcmcia_get_irq_info,
  configure_socket:	pfs168_pcmcia_configure_socket,

  socket_init:		sa1111_pcmcia_socket_init,
  socket_suspend:	sa1111_pcmcia_socket_suspend,
};
