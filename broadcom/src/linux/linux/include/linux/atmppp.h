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
/* atmppp.h - RFC2364 PPPoATM */

/* Written 2000 by Mitchell Blank Jr */

#ifndef _LINUX_ATMPPP_H
#define _LINUX_ATMPPP_H

#include <linux/atm.h>

#define PPPOATM_ENCAPS_AUTODETECT	(0)
#define PPPOATM_ENCAPS_VC		(1)
#define PPPOATM_ENCAPS_LLC		(2)

/*
 * This is for the ATM_SETBACKEND call - these are like socket families:
 * the first element of the structure is the backend number and the rest
 * is per-backend specific
 */
struct atm_backend_ppp {
	atm_backend_t	backend_num;	/* ATM_BACKEND_PPP */
	int		encaps;		/* PPPOATM_ENCAPS_* */
};

#endif	/* _LINUX_ATMPPP_H */
