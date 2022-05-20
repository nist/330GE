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
#ifndef _LMC_PROTO_H_
#define _LMC_PROTO_H_

void lmc_proto_init(lmc_softc_t *sc);
void lmc_proto_attach(lmc_softc_t *sc);
void lmc_proto_detach(lmc_softc_t *sc);
void lmc_proto_reopen(lmc_softc_t *sc);
int lmc_proto_ioctl(lmc_softc_t *sc, struct ifreq *ifr, int cmd);
void lmc_proto_open(lmc_softc_t *sc);
void lmc_proto_close(lmc_softc_t *sc);
unsigned short lmc_proto_type(lmc_softc_t *sc, struct sk_buff *skb);
void lmc_proto_netif(lmc_softc_t *sc, struct sk_buff *skb);
int lmc_skb_rawpackets(char *buf, char **start, off_t offset, int len, int unused);

#endif

