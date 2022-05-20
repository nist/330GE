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
 * RoboSwitch setup functions
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: bcmrobo.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _bcm_robo_h_
#define _bcm_robo_h_

/* Forward declaration */
typedef struct robo_info_s robo_info_t;

/* Device access/config oprands */
typedef struct {
	/* low level routines */
	void (*enable_mgmtif)(robo_info_t *robo);	/* enable mgmt i/f, optional */
	void (*disable_mgmtif)(robo_info_t *robo);	/* disable mgmt i/f, optional */
	int (*write_reg)(robo_info_t *robo, uint8 page, uint8 reg, void *val, int len);
	int (*read_reg)(robo_info_t *robo, uint8 page, uint8 reg, void *val, int len);
	/* description */
	char *desc;
} dev_ops_t;


typedef	uint16 (*miird_f)(void *h, int add, int off);
typedef	void (*miiwr_f)(void *h, int add, int off, uint16 val);

/* Private state per RoboSwitch */
struct robo_info_s {
	sb_t	*sbh;			/* SiliconBackplane handle */
	char	*vars;			/* nvram variables handle */
	void	*h;			/* dev handle */
	uint16	devid;			/* Device id for the switch */

	dev_ops_t *ops;			/* device ops */
	uint8	page;			/* current page */

	/* SPI */
	uint32	ss, sck, mosi, miso;	/* GPIO mapping */

	/* MII */
	miird_f	miird;
	miiwr_f	miiwr;
};

extern robo_info_t *bcm_robo_attach(sb_t *sbh, void *h, char *vars, miird_f miird, miiwr_f miiwr);
extern void bcm_robo_detach(robo_info_t *robo);
extern int bcm_robo_enable_device(robo_info_t *robo);
extern int bcm_robo_config_vlan(robo_info_t *robo);
extern int bcm_robo_enable_switch(robo_info_t *robo);


#endif /* _bcm_robo_h_ */
