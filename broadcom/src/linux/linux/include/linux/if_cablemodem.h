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
#ifndef _LINUX_CABLEMODEM_H_
#define _LINUX_CABLEMODEM_H_
/*
 *		Author: Franco Venturi <fventuri@mediaone.net>
 *		Copyright 1998 Franco Venturi
 *
 *		This program is free software; you can redistribute it
 *		and/or  modify it under  the terms of  the GNU General
 *		Public  License as  published  by  the  Free  Software
 *		Foundation;  either  version 2 of the License, or  (at
 *		your option) any later version.
 */

/* some useful defines for sb1000.c e cmconfig.c - fv */
#define SIOCGCMSTATS		SIOCDEVPRIVATE+0	/* get cable modem stats */
#define SIOCGCMFIRMWARE		SIOCDEVPRIVATE+1	/* get cm firmware version */
#define SIOCGCMFREQUENCY	SIOCDEVPRIVATE+2	/* get cable modem frequency */
#define SIOCSCMFREQUENCY	SIOCDEVPRIVATE+3	/* set cable modem frequency */
#define SIOCGCMPIDS			SIOCDEVPRIVATE+4	/* get cable modem PIDs */
#define SIOCSCMPIDS			SIOCDEVPRIVATE+5	/* set cable modem PIDs */

#endif
