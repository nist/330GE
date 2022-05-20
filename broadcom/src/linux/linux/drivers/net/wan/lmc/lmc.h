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
#ifndef _LMC_H_
#define _LMC_H_

#include "lmc_var.h"

/*
 * prototypes for everyone
 */
int lmc_probe(struct net_device * dev);
unsigned lmc_mii_readreg(lmc_softc_t * const sc, unsigned
      			  devaddr, unsigned regno);
void lmc_mii_writereg(lmc_softc_t * const sc, unsigned devaddr,
			       unsigned regno, unsigned data);
void lmc_led_on(lmc_softc_t * const, u_int32_t);
void lmc_led_off(lmc_softc_t * const, u_int32_t);
unsigned lmc_mii_readreg(lmc_softc_t * const, unsigned, unsigned);
void lmc_mii_writereg(lmc_softc_t * const, unsigned, unsigned, unsigned);
void lmc_gpio_mkinput(lmc_softc_t * const sc, u_int32_t bits);
void lmc_gpio_mkoutput(lmc_softc_t * const sc, u_int32_t bits);

int lmc_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);

extern lmc_media_t lmc_ds3_media;
extern lmc_media_t lmc_ssi_media;
extern lmc_media_t lmc_t1_media;
extern lmc_media_t lmc_hssi_media;

#ifdef _DBG_EVENTLOG
static void lmcEventLog( u_int32_t EventNum, u_int32_t arg2, u_int32_t arg3 );
#endif

#endif

